#include "../../common/network/include/network.h"
#include "../include/game_handler.h"
#include "../include/server.h"
#include "messages_type.h"
#include "vector"

int main() {
    war_of_ages::server::server::set_port(12345);
    war_of_ages::server::server::instance().start();
    while (true) {
        war_of_ages::server::server::instance().update(-1, true);
    }

    //    std::thread([]() {
    //    }).detach();
    //    ///  Looks normal for both clients
    //    while (true) {
    //    }
    ///    For other client (not host) looks like very small fps
    ///    +- normal image if replace 10 with number <= 3 (wtf ???)
    //    include <chrono>
    //    ...
    //    std::chrono::time_point last_sending = std::chrono::system_clock::now();
    //    while (true) {
    //        std::chrono::time_point current_sending = std::chrono::system_clock::now();
    //        using ms = std::chrono::duration<float, std::milli>;
    //        if (std::chrono::duration_cast<ms>(current_sending - last_sending).count() >= 10) {
    //            war_of_ages::server::game_handler::instance().update_games();
    //            last_sending = current_sending;
    //        }
    //    }

    ///  Crashes with uncaught exception ( .at() )
    //    #include <chrono>
    //    ...
    //    while (true) {
    //        std::chrono::time_point start = std::chrono::system_clock::now();
    //        war_of_ages::server::game_handler::instance().update_games();
    //        std::chrono::time_point end = std::chrono::system_clock::now();
    //        std::this_thread::sleep_for(std::chrono::milliseconds(17 - (end - start).count()));
    //    }
}
