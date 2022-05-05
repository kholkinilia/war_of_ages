#include "../include/bot_actions_receiver.h"
#include "../include/client.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> const &bot_actions_receiver::get_actions(player_snapshot p) {
    actions.clear();
    for (int i = 0; i < p.money / unit::get_stats(unit_type::PEASANT).cost; i++) {
        append(std::make_unique<buy_unit_command>(static_cast<int>(unit_type::PEASANT)));
    }

    return actions;
}

}  // namespace war_of_ages