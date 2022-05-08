#include "../include/bot_actions_supplier.h"

namespace war_of_ages::client {

std::vector<std::unique_ptr<game_command>> bot_actions_supplier::get_actions(
    const std::pair<player_snapshot, player_snapshot> &snapshot) {
    auto p = snapshot.first;
    p.money = 100;
    std::vector<std::unique_ptr<game_command>> actions;
    actions.reserve(p.money / unit::get_stats(unit_type::PEASANT).cost);
    for (int i = 0; i < p.money / unit::get_stats(unit_type::PEASANT).cost; i++) {
        actions.emplace_back(std::make_unique<buy_unit_command>(static_cast<int>(unit_type::PEASANT)));
    }
    return actions;
}

bot_actions_supplier &bot_actions_supplier::instance() {
    static bot_actions_supplier supplier;
    return supplier;
}
}  // namespace war_of_ages::client