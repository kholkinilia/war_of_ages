#ifndef WAR_OF_AGES_GAME_STATE_H
#define WAR_OF_AGES_GAME_STATE_H

#include <vector>
#include "game_commands.h"
#include "player.h"

namespace war_of_ages {

struct game_state {
private:
    player p1, p2;

    static void apply_commands(player &p, const std::vector<game_command> &commands);

public:
    void update(const std::vector<game_command> &p1_commands,
                const std::vector<game_command> &p2_commands,
                int dt);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_STATE_H
