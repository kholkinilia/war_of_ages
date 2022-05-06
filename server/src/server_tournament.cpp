#include "../include/server_tournament.h"
#include <algorithm>
#include <random>
#include "../include/game_handler.h"

void war_of_ages::server_tournament::post_add_participant(const std::string &handle) {
    ready_to_play_participants.push_back(handle);
    match_participants();
}

void war_of_ages::server_tournament::post_add_result(const std::string &winner, const std::string &loser) {
    ready_to_play_participants.push_back(winner);
    ready_to_play_participants.push_back(loser);
    for (const auto &handle : m_participants) {
        // TODO: send respond
    }
    match_participants();
}

void war_of_ages::server_tournament::post_remove_participant(const std::string &handle) {
    ready_to_play_participants.erase(
        std::remove(ready_to_play_participants.begin(), ready_to_play_participants.end(), handle),
        ready_to_play_participants.end());
}

void war_of_ages::server_tournament::match_participants() {
    static std::mt19937_64 rnd(clock());
    while (ready_to_play_participants.size() >= 2) {
        std::size_t p2 = rnd() % (ready_to_play_participants.size() - 1) + 1;
        std::size_t p1 = rnd() % p2;
        //        game_handler::instance().add_game(ready_to_play_participants[p1],
        //                                          ready_to_play_participants[p2]);  // TODO: pass
        //                                          post_game_actions
        std::swap(ready_to_play_participants[p2], ready_to_play_participants.back());
        ready_to_play_participants.pop_back();
        std::swap(ready_to_play_participants[p1], ready_to_play_participants.back());
        ready_to_play_participants.pop_back();
    }
}
