#include "../include/tournament.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
namespace war_of_ages {

void tournament::update_places_lock_held() {
    std::vector<std::size_t> cnt(m_participants.size() * WIN_POINTS + 1);
    std::vector<std::size_t> final_place(m_participants.size() * WIN_POINTS + 1);
    for (std::size_t i = 0; i < m_participants.size(); i++) {
        cnt[m_sum[i]]++;
    }
    std::size_t cur_place = 1;
    for (std::size_t i = m_participants.size() * WIN_POINTS; i + 1 != 0; i--) {
        final_place[i] = cur_place;
        cur_place += cnt[i];
    }

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        m_place[i] = final_place[m_sum[i]];
    }
}

void tournament::add_participant(const std::string &handle) {
    std::unique_lock lock(m_mutex);
    std::size_t id = get_id(handle);
    if (id != m_participants.size()) {
        assert(false);
        return;
    }
    m_participants.push_back(handle);
    for (auto &participant_results : m_match_results) {
        participant_results.emplace_back(game_result::NONE);
    }
    m_match_results.emplace_back(m_participants.size() + 1);
    m_sum.emplace_back(0);
    m_place.emplace_back(0);

    update_places_lock_held();

    post_add_participant(handle);
}

void tournament::add_result(const std::string &winner, const std::string &loser) {
    std::unique_lock lock(m_mutex);
    std::size_t winner_id = get_id(winner);
    std::size_t loser_id = get_id(loser);
    m_match_results[winner_id][loser_id] = game_result::VICTORY;
    m_match_results[loser_id][winner_id] = game_result::DEFEAT;
    m_sum[winner_id] += WIN_POINTS;
    update_places_lock_held();

    post_add_result(winner, loser);
}

void tournament::remove_participant(const std::string &handle) {
    std::unique_lock lock(m_mutex);
    auto remove_id = static_cast<long long>(get_id(handle));

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        if (m_match_results[remove_id][i] == game_result::DEFEAT) {
            m_sum[i] -= WIN_POINTS;
        }
    }
    for (auto &part_results : m_match_results) {
        part_results.erase(part_results.begin() + remove_id);
    }
    m_match_results.erase(m_match_results.begin() + remove_id);
    m_sum.erase(m_sum.begin() + remove_id);
    m_place.erase(m_place.begin() + remove_id);
    m_participants.erase(m_participants.begin() + remove_id);
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

std::size_t tournament::get_id(const std::string &handle) const noexcept {
    return std::find(m_participants.begin(), m_participants.end(), handle) - m_participants.begin();
}

tournament_snapshot tournament::get_snapshot() const {
    return {m_name, m_key, m_participants, m_match_results};
}

}  // namespace war_of_ages
