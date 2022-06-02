#ifndef WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H
#define WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H

#include <memory>
#include <random>
#include <set>
#include <vector>
#include "../../game_logic/include/game_commands.h"

namespace war_of_ages::client {

struct state {
    std::pair<int, int> damage = {-1, -1};
    state() = default;
    state(int a, int b) : damage(a, b) {
    }
};

struct bot_actions_supplier {
private:
    bot_actions_supplier();
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

    static inline std::vector<std::vector<std::vector<float>>> Q_table;
    state last_state;
    action last_action = action::NONE;
    const float learning_rate = 0.07, gamma = 0.85;
    static bool read;

    static action get_action(state state);
    static state get_state(const std::pair<player_snapshot, player_snapshot> &p);

public:
    bot_actions_supplier(const bot_actions_supplier &) = delete;
    bot_actions_supplier(bot_actions_supplier &&) = delete;
    bot_actions_supplier &operator=(const bot_actions_supplier &) = delete;
    bot_actions_supplier &operator=(bot_actions_supplier &&) = delete;

    ~bot_actions_supplier() = default;

    [[nodiscard]] std::vector<std::unique_ptr<game_command>> get_actions(
        const std::pair<player_snapshot, player_snapshot> &p);

    static void read_from_file();
    static void write_to_file();

    static bot_actions_supplier &instance();
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H
