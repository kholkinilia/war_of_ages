#ifndef WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H
#define WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H

#include <memory>
#include <random>
#include <set>
#include <vector>
#include "../../game_logic/include/game_commands.h"
#include "actions_receiver.h"

namespace war_of_ages {

struct state {
    std::pair<int, int> damage = {-1, -1};
    state() = default;
    state(int a, int b) : damage(a, b) {}
};

struct bot_actions_receiver : actions_receiver {
public:
    bot_actions_receiver();

    std::vector<std::unique_ptr<game_command>> const &get_actions(
        std::pair<player_snapshot, player_snapshot> p,
        int player) final;

    static void write_to_file(int player);
    static void read_from_file(int player);

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

    static inline std::vector<std::vector<std::vector<std::vector<float>>>> Q_table;
    static inline std::vector<std::set<std::pair<int, int>>> positions;
    state last_state;
    action last_action = action::NONE;
    const float learning_rate = 0.07, gamma = 0.85;
    static bool read;

    static action get_action(state state, int player);
    static state get_state(const std::pair<player_snapshot, player_snapshot> &p, int player);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H
