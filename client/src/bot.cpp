#include "../include/bot.h"
#include "../include/client.h"

namespace war_of_ages {

void generate_actions(int index, const player_snapshot &player) {
    for (int i = 0; i < player.money / unit::get_stats(unit_type::PEASANT).cost; i++) {
        current_state.add_action(index,
                                 std::make_unique<buy_unit_command>(static_cast<int>(unit_type::PEASANT)));
    }
}

}  // namespace war_of_ages