#ifndef WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H
#define WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H

#include <memory>
#include <vector>
#include "../../common/game_logic/include/game_commands.h"

namespace war_of_ages {

struct bot_actions_supplier {
private:
    bot_actions_supplier() = default;

public:
    bot_actions_supplier(const bot_actions_supplier &) = delete;
    bot_actions_supplier(bot_actions_supplier &&) = delete;
    bot_actions_supplier &operator=(const bot_actions_supplier &) = delete;
    bot_actions_supplier &operator=(bot_actions_supplier &&) = delete;

    ~bot_actions_supplier() = default;

    [[nodiscard]] static std::vector<std::unique_ptr<game_command>> get_actions(
        const std::pair<player_snapshot, player_snapshot> &snapshot);

    static bot_actions_supplier &instance();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H
