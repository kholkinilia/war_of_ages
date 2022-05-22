#include "../include/server_tournament.h"
#include <algorithm>
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
    match_participants_lock_held();
}

void war_of_ages::server::server_tournament::post_add_result(const std::string &winner,
                                                             const std::string &loser) {
    //    std::cerr << "Made it up to post_add_result" << std::endl;
    m_is_playing[get_id(winner)] = m_is_playing[get_id(loser)] = false;

    message<messages_type> msg;
    msg.header.id = messages_type::TOURNAMENT_ADD_RESULT;

    msg.insert_container(winner);
    msg.insert_container(loser);

    for (const auto &handle : m_participants) {
        //        std::cerr << "Sending the result to " << handle << std::endl;
        server::instance().send_message(handle, msg);
    }
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
}

void war_of_ages::server::server_tournament::match_participants_lock_held() {
    std::vector<std::pair<std::size_t, std::size_t>> order(m_participants.size());
    std::vector<std::size_t> games_played(m_participants.size());

    //    std::cerr << "Tournament(" << m_key << ") matchmaking started..." << std::endl;

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        order[i].first = games_played[i] =
            m_participants.size() -
            std::count(m_match_results[i].begin(), m_match_results[i].end(), game_result::NONE);
        order[i].second = i;
        //        std::cerr << i << ": " << m_is_playing[i] << " " << m_participants[i] << " " <<
        //        games_played[i]
        //                  << std::endl;
        //        std::cerr << std::count(m_match_results[i].begin(), m_match_results[i].end(),
        //        game_result::NONE) << std::endl; std::cerr << "size: " << m_match_results[i].size() <<
        //        std::endl; for (std::size_t j = 0; j < m_match_results[i].size(); j++) {
        //            std::cerr << (int)m_match_results[i][j] << " ";
        //        }
        //        std::cerr << std::endl;
    }

    std::sort(order.begin(), order.end());

    std::vector<std::pair<std::size_t, std::size_t>> new_matches;

    for (auto &[gp, i] : order) {
        //        std::cerr << "Searching for match for " << m_participants[i] << std::endl;
        std::size_t pos_min = m_participants.size();
        std::size_t games_min = m_participants.size();
        for (std::size_t j = 0; j < m_participants.size(); j++) {
            //            std::cerr << "Maybe " << m_participants[j] << "?" << std::endl;
            if (i == j) {
                //                std::cerr << "No, this is the same participant." << std::endl;
                continue;
            }
            if (m_is_playing[j]) {
                //                std::cerr << "No, the participant is playing." << std::endl;
                continue;
            }
            if (m_match_results[i][j] != game_result::NONE) {
                //                std::cerr << "No, they have already played." << std::endl;
                continue;
            }
            //            if (i == j || m_is_playing[j] || m_match_results[i][j] != game_result::NONE)
            //                continue;
            if (games_min > games_played[j]) {
                //                std::cerr << "Yes, this is a good match!" << std::endl;
                games_min = games_played[j];
                pos_min = j;
            } else {
                //                std::cerr << "No, we have a better alternative." << std::endl;
            }
        }
        if (pos_min != m_participants.size()) {
            m_is_playing[i] = m_is_playing[pos_min] = true;
            new_matches.emplace_back(i, pos_min);
        }
    }

    auto post_game_actions = [](const std::string &winner,
                                const std::string &loser) {  // FIXME: doesn't work somehow
        tournament_handler::instance().add_result(winner, loser);
    };

    for (auto [player1_id, player2_id] : new_matches) {
        //        std::cerr << "Matching " << m_participants[player1_id] << " " << m_participants[player2_id]
        //        << "\n";
        game_handler::instance().add_game(m_participants[player1_id], m_participants[player2_id],
                                          post_game_actions);
    }

    //    std::cerr << "Tournament(" << m_key << ") matchmaking ended." << std::endl;
}
