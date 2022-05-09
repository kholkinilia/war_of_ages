#include "../../common/network/include/network.h"
#include "../include/game_handler.h"
#include "../include/server.h"

int main() {
    war_of_ages::server::server::set_port(12345);
    war_of_ages::server::server::instance().start();
    std::thread([]() {
        while (true) {
            war_of_ages::server::server::instance().update(-1, true);
        }
    }).detach();
    while (true) {
        int n = 3000000;
        for (long long i = 0; i != n + 1; ++i) {
            if (i == n) {
                i = 0;
                war_of_ages::server::game_handler::instance().update_games();
            }
        }
    }
}
