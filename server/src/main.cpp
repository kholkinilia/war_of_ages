#include "../../common/network/include/network.h"
#include "../include/server.h"

int main() {
    war_of_ages::server::server::set_port(12345);
    war_of_ages::server::server::instance().start();
    while (true) {
        war_of_ages::server::server::instance().update(-1, true);
    }
}
