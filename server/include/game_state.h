#ifndef WAR_OF_AGES_GAME_STATE_H
#define WAR_OF_AGES_GAME_STATE_H

#include <vector>
#include "../include/player.h"

namespace war_of_ages {

enum class command {};  // TODO: think of a decent commands format

struct game_state {
private:
    player p1, p2;

public:
    void update(std::vector<command> p1_commands, std::vector<command> p2_commands, int dt);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_STATE_H
