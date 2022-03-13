#ifndef WAR_OF_AGES_GAME_STATE_H
#define WAR_OF_AGES_GAME_STATE_H

#include <vector>
#include "game_commands.h"
#include "player.h"
#include <memory>

namespace war_of_ages {

struct game_state {
private:
    player p1, p2;

public:
    void update(const std::vector<std::unique_ptr<game_command>> &p1_commands,
                const std::vector<std::unique_ptr<game_command>> &p2_commands,
                double dt);
    std::pair<player_snapshot, player_snapshot> snapshot_players();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_STATE_H
