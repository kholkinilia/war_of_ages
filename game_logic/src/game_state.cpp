#include "../include/game_state.h"
#include <mutex>
#include <vector>
#include "../include/game_commands.h"
#include "../include/player.h"

namespace war_of_ages {

void game_state::update(const std::vector<game_command> &p1_commands,
                        const std::vector<game_command> &p2_commands,
                        double dt) {
    p1.update(p2, dt);
    p2.update(p1, dt);

    apply_commands(p1, p1_commands);
    apply_commands(p2, p2_commands);

    p1.clear_dead_objects();
    p2.clear_dead_objects();
}

void game_state::apply_commands(player &p, const std::vector<game_command> &commands) {
    for (auto cmd : commands) {
        switch (cmd.m_command) {
            case command::BUY_UNIT:
                p.buy_unit(cmd.m_param[0]);
            case command::BUY_CANNON:
                p.buy_cannon(cmd.m_param[0], cmd.m_param[1]);
                break;
            case command::SELL_CANNON:
                p.sell_cannon(cmd.m_param[0]);
            case command::BUY_CANNON_SLOT:
                p.buy_cannon_slot();
            case command::USE_ULT:
                p.use_ult();
        }
    }
}

}  // namespace war_of_ages