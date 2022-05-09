#include "../include/server_tournament.h"
#include <algorithm>
#include <random>
#include "../../common/include/messages_type.h"
#include "../include/game_handler.h"
#include "../include/tournament_handler.h"

void war_of_ages::server::server_tournament::post_add_participant(const std::string &handle) {
    ready_to_play_participants.push_back(handle);

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
    msg.insert_container(handle);

    for (const auto &part : m_participants) {
        if (part == handle) {
            continue;
        }
        server::instance().send_message(part, msg);
    }
    match_participants();
}

void war_of_ages::server::server_tournament::post_add_result(const std::string &winner,
                                                             const std::string &loser) {
    ready_to_play_participants.push_back(winner);
    ready_to_play_participants.push_back(loser);
    message<messages_type> msg;
    msg.header.id = messages_type::TOURNAMENT_ADD_RESULT;

    msg.insert_container(winner);
    msg.insert_container(loser);

    for (const auto &handle : m_participants) {
        server::instance().send_message(handle, msg);
    }
    match_participants();
}

void war_of_ages::server::server_tournament::post_remove_participant(const std::string &handle) {
    ready_to_play_participants.erase(
        std::remove(ready_to_play_participants.begin(), ready_to_play_participants.end(), handle),
        ready_to_play_participants.end());

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

void war_of_ages::server::server_tournament::match_participants() {  // TODO: make it more intelligent
    std::unique_lock lock(m_mutex);
    static std::mt19937_64 rnd(clock());
    while (ready_to_play_participants.size() >= 2) {
        std::size_t p2 = rnd() % (ready_to_play_participants.size() - 1) + 1;
        std::size_t p1 = rnd() % p2;

        auto post_game_actions = [](const std::string &winner, const std::string &loser) {
            tournament_handler::instance().add_result(winner, loser);
        };

        game_handler::instance().add_game(ready_to_play_participants[p1], ready_to_play_participants[p2],
                                          post_game_actions);
        std::swap(ready_to_play_participants[p2], ready_to_play_participants.back());
        ready_to_play_participants.pop_back();
        std::swap(ready_to_play_participants[p1], ready_to_play_participants.back());
        ready_to_play_participants.pop_back();
    }
}
