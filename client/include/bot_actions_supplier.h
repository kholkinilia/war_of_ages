#ifndef WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H
#define WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H

#include <memory>
#include <vector>
#include "../../common/game_logic/include/game_commands.h"
#include "actions_supplier.h"

namespace war_of_ages {

struct bot_actions_supplier : actions_supplier {
    std::vector<std::unique_ptr<game_command>> pop_actions() final;
    void add_action(std::unique_ptr<game_command>) final;
    void clear_actions() final;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BOT_ACTIONS_SUPPLIER_H
