#include "../include/bot.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> generate_actions(const player_snapshot &player) {
    std::vector<std::unique_ptr<game_command>> actions(player.money /
                                                       unit::get_stats(unit_type::PEASANT).cost);
    for (int i = 0; i < player.money / unit::get_stats(unit_type::PEASANT).cost; i++) {
        actions[i] = std::make_unique<buy_unit_command>(static_cast<int>(unit_type::PEASANT));
    }
    return actions;
}

}  // namespace war_of_ages