#include "../include/tournament.h"
#include <cassert>

namespace war_of_ages {

void tournament::update_places_lock_held() {
    std::vector<int> cnt(part_number * WIN_POINTS + 1);
    std::vector<int> final_place(part_number * WIN_POINTS + 1);
    for (int i = 0; i < part_number; i++) {
        cnt[sum[i]]++;
    }
    int cur_place = 1;
    for (int i = part_number * WIN_POINTS; i >= 0; i--) {
        final_place[i] = cur_place;
        cur_place += cnt[i];
    }

    for (int i = 0; i < part_number; i++) {
        place[i] = final_place[sum[i]];
    }
}

void tournament::add_participant(const std::string &handle) {
    std::unique_lock lock(m);
    if (id.count(handle) != 0) {
        assert(false);
        return;
    }
    participants.push_back(handle);
    id[handle] = part_number;
    for (auto &participant_results : match_results) {
        participant_results.emplace_back(result::NONE);
    }
    match_results.emplace_back(part_number + 1);
    sum.emplace_back(0);
    place.emplace_back(0);
    part_number++;
    is_grid_updated = false;
    update_places_lock_held();
}

void tournament::add_result(const std::string &winner, const std::string &loser) {
    std::unique_lock lock(m);
    match_results[id[winner]][id[loser]] = result::VICTORY;
    match_results[id[loser]][id[winner]] = result::DEFEAT;
    sum[id[winner]] += WIN_POINTS;
    is_grid_updated = false;
    update_places_lock_held();
}

void tournament::remove_participant(const std::string &handle) {
    std::unique_lock lock(m);
    int remove_id = id[handle];
    for (int i = 0; i < part_number; i++) {
        if (match_results[remove_id][i] == result::DEFEAT) {
            sum[i] -= WIN_POINTS;
        }
    }
    for (auto &part_results : match_results) {
        part_results.erase(part_results.begin() + remove_id);
    }
    for (auto &cur_handle : participants) {
        if (id[cur_handle] > remove_id) {
            id[cur_handle]--;
        }
    }
    id.erase(participants[remove_id]);
    match_results.erase(match_results.begin() + remove_id);
    sum.erase(sum.begin() + remove_id);
    place.erase(place.begin() + remove_id);
    participants.erase(participants.begin() + remove_id);
    part_number--;
    is_grid_updated = false;
    update_places_lock_held();
}

}  // namespace war_of_ages
