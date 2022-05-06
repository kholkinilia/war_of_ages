#include <iostream>
#include "../../common/network/include/network.h"
#include "../include/server.h"
#include "../include/game_handler.h"

int main() {
    std::ios_base::sync_with_stdio(0); std::cin.tie(0); std::cout.tie(0);
    war_of_ages::server::set_port(12345);
    war_of_ages::server::instance().start();
    std::thread([]() {
        while (true) {
            war_of_ages::server::instance().update(-1, true);
        }
    }).detach();
    while (true) {
        war_of_ages::game_handler::instance().update_games();
    }
}
