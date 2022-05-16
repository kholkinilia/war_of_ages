#include "../include/player_actions_receiver.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> const &player_actions_receiver::get_actions(
    std::pair<player_snapshot, player_snapshot>,
    int) {
    return actions;
}

}  // namespace war_of_ages