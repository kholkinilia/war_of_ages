#include "../include/tournament.h"
#include <cassert>

namespace war_of_ages {

void tournament::update_places_lock_held() {
    std::vector<int> cnt(m_part_number * WIN_POINTS + 1);
    std::vector<int> final_place(m_part_number * WIN_POINTS + 1);
    for (int i = 0; i < m_part_number; i++) {
        cnt[m_sum[i]]++;
    }
    int cur_place = 1;
    for (int i = m_part_number * WIN_POINTS; i >= 0; i--) {
        final_place[i] = cur_place;
        cur_place += cnt[i];
    }

    for (int i = 0; i < m_part_number; i++) {
        m_place[i] = final_place[m_sum[i]];
    }
}

void tournament::add_participant(const std::string &handle) {
    std::unique_lock lock(m_mutex);
    if (m_id.count(handle) != 0) {
        assert(false);
        return;
    }
    m_participants.push_back(handle);
    m_id[handle] = m_part_number;
    for (auto &participant_results : m_match_results) {
        participant_results.emplace_back(result::NONE);
    }
    m_match_results.emplace_back(m_part_number + 1);
    m_sum.emplace_back(0);
    m_place.emplace_back(0);
    m_part_number++;
    update_places_lock_held();

    post_add_participant(handle);
}

void tournament::add_result(const std::string &winner, const std::string &loser) {
    std::unique_lock lock(m_mutex);
    m_match_results[m_id[winner]][m_id[loser]] = result::VICTORY;
    m_match_results[m_id[loser]][m_id[winner]] = result::DEFEAT;
    m_sum[m_id[winner]] += WIN_POINTS;
    update_places_lock_held();

    post_add_result(winner, loser);
}

void tournament::remove_participant(const std::string &handle) {
    std::unique_lock lock(m_mutex);
    int remove_id = m_id[handle];
    for (int i = 0; i < m_part_number; i++) {
        if (m_match_results[remove_id][i] == result::DEFEAT) {
            m_sum[i] -= WIN_POINTS;
        }
    }
    for (auto &part_results : m_match_results) {
        part_results.erase(part_results.begin() + remove_id);
    }
    for (auto &cur_handle : m_participants) {
        if (m_id[cur_handle] > remove_id) {
            m_id[cur_handle]--;
        }
    }
    m_id.erase(m_participants[remove_id]);
    m_match_results.erase(m_match_results.begin() + remove_id);
    m_sum.erase(m_sum.begin() + remove_id);
    m_place.erase(m_place.begin() + remove_id);
    m_participants.erase(m_participants.begin() + remove_id);
    m_part_number--;
    update_places_lock_held();

    post_remove_participant(handle);
}

void tournament::set_name(const std::string &new_name) {
    std::unique_lock lock(m_mutex);
    m_name = new_name;
}

void tournament::set_key(const std::string &new_key) {
    std::unique_lock lock(m_mutex);
    m_key = new_key;
}

std::string tournament::get_key() const {
    std::unique_lock lock(m_mutex);
    return m_key;
}

std::string tournament::get_name() const {
    std::unique_lock lock(m_mutex);
    return m_name;
}

}  // namespace war_of_ages
