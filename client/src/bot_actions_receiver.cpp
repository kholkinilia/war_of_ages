#include "../include/bot_actions_receiver.h"
#include "../include/client.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> const &bot_actions_receiver::get_actions(
    std::pair<player_snapshot, player_snapshot> p,
    int player) {
    actions.clear();
    int money = player == 0 ? p.first.money : p.second.money;
    for (int i = 0; i < money / unit::get_stats(unit_type::PEASANT).cost; i++) {
        append(std::make_unique<buy_unit_command>(static_cast<int>(unit_type::PEASANT)));
    }

    return actions;
}

}  // namespace war_of_ages