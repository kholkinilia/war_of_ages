#include "../include/server_tournament.h"
#include <algorithm>
#include "../include/chat_handler.h"
#include "../include/game_handler.h"
#include "../include/tournament_handler.h"
#include "messages_type.h"

void war_of_ages::server::server_tournament::post_add_participant(const std::string &handle) {
    m_is_playing.push_back(false);

    message<messages_type> msg_to_handle;
    msg_to_handle.header.id = messages_type::TOURNAMENT_STATE;
    auto snapshot = get_snapshot();

    msg_to_handle << snapshot.key;
    msg_to_handle << snapshot.name;
    msg_to_handle << snapshot.participants;
    msg_to_handle << snapshot.match_results;

    server::instance().send_message(handle, msg_to_handle);

    message<messages_type> msg;
    msg.header.id = messages_type::TOURNAMENT_ADD_PLAYER;
    msg << handle;

    for (const auto &part : m_participants) {
        if (part == handle) {
            continue;
        }
        server::instance().send_message(part, msg);
    }
    chat_handler::instance().add_system_message(m_key, "[TOURNAMENT]", handle + " joined the tournament.");
    match_participants_lock_held();
}

void war_of_ages::server::server_tournament::post_add_result(const std::string &winner,
                                                             const std::string &loser) {
    //    std::cerr << "Made it up to post_add_result" << std::endl;

    message<messages_type> msg;
    msg.header.id = messages_type::TOURNAMENT_ADD_RESULT;

    msg.insert_container(winner);
    msg.insert_container(loser);

    for (const auto &handle : m_participants) {
        //        std::cerr << "Sending the result to " << handle << std::endl;
        server::instance().send_message(handle, msg);
    }
    chat_handler::instance().add_system_message(m_key, "[TOURNAMENT]", winner + " defeated " + loser + ".");
    match_participants_lock_held();
}

void war_of_ages::server::server_tournament::post_remove_participant(const std::string &handle,
                                                                     std::size_t remove_id) {
    if (remove_id < m_participants.size()) {
        m_is_playing.erase(m_is_playing.begin() + static_cast<long long>(remove_id));
    }

    for (const auto &part : m_participants) {
        if (part == handle) {
            continue;
        }
        message<messages_type> msg;
        msg.header.id = messages_type::TOURNAMENT_REMOVE_PLAYER;
        msg.insert_container(handle);

        server::instance().send_message(part, msg);
    }
    chat_handler::instance().add_system_message(m_key, "[TOURNAMENT]", handle + " left the tournament.");
    match_participants_lock_held();
}

void war_of_ages::server::server_tournament::match_participants_lock_held() {
    std::vector<std::pair<std::size_t, std::size_t>> order(m_participants.size());
    std::vector<std::size_t> games_played(m_participants.size());

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        order[i].first = games_played[i] =
            m_participants.size() -
            std::count(m_match_results[i].begin(), m_match_results[i].end(), game_result::NONE);
        order[i].second = i;
    }

    std::sort(order.begin(), order.end());

    std::vector<std::pair<std::size_t, std::size_t>> new_matches;

    for (auto &[gp, i] : order) {
        if (m_is_playing[i]) {
            continue;
        }
        std::size_t pos_min = m_participants.size();
        std::size_t games_min = m_participants.size();
        for (std::size_t j = 0; j < m_participants.size(); j++) {
            if (i == j || m_is_playing[j] || m_match_results[i][j] != game_result::NONE)
                continue;
            if (games_min > games_played[j]) {
                games_min = games_played[j];
                pos_min = j;
            }
        }
        if (pos_min != m_participants.size()) {
            m_is_playing[i] = m_is_playing[pos_min] = true;
            new_matches.emplace_back(i, pos_min);
        }
    }

    for (auto [player1_id, player2_id] : new_matches) {
        m_match_results[player1_id][player2_id] = m_match_results[player2_id][player1_id] =
            game_result::PLAYING;
        post_match_participants(m_participants[player1_id], m_participants[player2_id]);
    }
}

void war_of_ages::server::server_tournament::post_match_participants(const std::string &handle1,
                                                                     const std::string &handle2) {
    auto post_game_actions = [this](const std::string &winner,
                                    const std::string &loser) {  // FIXME: doesn't work somehow
        std::unique_lock lock(m_mutex);
        if (get_id(winner) != m_participants.size()) {
            m_is_playing[get_id(winner)] = false;
        }
        if (get_id(loser) != m_participants.size()) {
            m_is_playing[get_id(loser)] = false;
        }
        lock.unlock();
        tournament_handler::instance().add_result(winner, loser);
    };
    game_handler::instance().add_game(handle1, handle2, post_game_actions);

    message<messages_type> msg;
    msg.header.id = messages_type::TOURNAMENT_ADD_MATCH;
    msg << handle1 << handle2;

    for (const auto &handle : m_participants) {
        if (handle == handle1 || handle == handle2) {
            continue;
        }
        server::instance().send_message(handle, msg);
    }
    chat_handler::instance().add_system_message(m_key, "[TOURNAMENT]",
                                                handle1 + " and " + handle2 + " are playing now.");
}
