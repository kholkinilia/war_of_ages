#ifndef WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H
#define WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H

#include <memory>
#include <vector>
#include "../../game_logic/include/game_commands.h"
#include "actions_receiver.h"

namespace war_of_ages {

struct bot_actions_receiver : actions_receiver {
    std::vector<std::unique_ptr<game_command>> const &get_actions(
        std::pair<player_snapshot, player_snapshot> p, int player) final;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BOT_ACTIONS_RECEIVER_H
