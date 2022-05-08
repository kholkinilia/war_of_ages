#include "../include/bot_actions_supplier.h"
#include "../include/client.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> bot_actions_supplier::pop_actions() {
    player_snapshot p;  // TODO : get it from game_handler, when implemented
    p.money = 100;
    std::vector<std::unique_ptr<game_command>> actions;
    actions.reserve(p.money / unit::get_stats(unit_type::PEASANT).cost);
    for (int i = 0; i < p.money / unit::get_stats(unit_type::PEASANT).cost; i++) {
        actions.emplace_back(std::make_unique<buy_unit_command>(static_cast<int>(unit_type::PEASANT)));
    }
    return actions;
}

void bot_actions_supplier::add_action(std::unique_ptr<game_command>) {
}

void bot_actions_supplier::clear_actions() {
}

}  // namespace war_of_ages