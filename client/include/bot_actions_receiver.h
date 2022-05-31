#ifndef WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H
#define WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H

#include <memory>
#include <random>
#include <vector>
#include "../../game_logic/include/game_commands.h"
#include "actions_receiver.h"

namespace war_of_ages {

struct state {
    std::pair<int, int> damage = {-1, -1}, hp = {-1, -1};
};

struct bot_actions_receiver : actions_receiver {
public:
    bot_actions_receiver();

    std::vector<std::unique_ptr<game_command>> const &get_actions(
        std::pair<player_snapshot, player_snapshot> p,
        int player) final;

    static std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> get_Q_table();

private:
    enum class action {
        BUY_UNIT_1,
        BUY_UNIT_2,
        BUY_UNIT_3,
        BUY_CANNON_1,
        BUY_CANNON_2,
        BUY_CANNON_3,
        SELL_CANNON_1,
        SELL_CANNON_2,
        SELL_CANNON_3,
        BUY_SLOT,
        NEW_ERA,
        USE_ULT,
        NONE
    };

    static inline std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> Q_table;
    state last_state;
    action last_action = action::NONE;
    const double learning_rate = 0.2, gamma = 0.9;

    static action get_action(state state);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H
