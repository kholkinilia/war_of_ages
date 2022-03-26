#include "../include/network_actions_receiver.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> const &network_actions_receiver::get_actions(player_snapshot) {
    // TODO: replace with network connection
    return actions;
}

}  // namespace war_of_ages