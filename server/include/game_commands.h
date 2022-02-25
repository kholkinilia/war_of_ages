#ifndef WAR_OF_AGES_GAME_COMMANDS_H
#define WAR_OF_AGES_GAME_COMMANDS_H

#include <vector>

enum class command {
    BUY_UNIT,        // m_param[0] = level;
    BUY_CANNON,      // m_param[0] = level; m_param[1] = slot;
    SELL_CANNON,     // m_param[0] = slot;
    BUY_CANNON_SLOT  // NO PARAMS
};

struct game_command {
    command m_command;
    std::vector<int> m_param;
};

#endif  // WAR_OF_AGES_GAME_COMMANDS_H
