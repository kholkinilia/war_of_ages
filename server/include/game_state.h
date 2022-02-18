#ifndef WAR_OF_AGES_GAME_STATE_H
#define WAR_OF_AGES_GAME_STATE_H

#include "../include/player.h"

namespace war_of_ages {

struct game_state {
private:
    player p1, p2;

public:
    void update(int dt);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_STATE_H
