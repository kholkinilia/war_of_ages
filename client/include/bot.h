#ifndef WAR_OF_AGES_BOT_H
#define WAR_OF_AGES_BOT_H

#include <memory>
#include <vector>
#include "../../game_logic/include/game_commands.h"

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> generate_actions(const player_snapshot &player);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BOT_H
