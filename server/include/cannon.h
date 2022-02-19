#ifndef WAR_OF_AGES_CANNON_H
#define WAR_OF_AGES_CANNON_H

#include "unit.h"

namespace war_of_ages {

enum cannon_type { STONE_LEVEL_1, STONE_LEVEL_2, STONE_LEVEL_3 };  // STONE -- age

struct cannon_stats {
    int cost;
    int cooldown;
    cannon_stats(int cost_, int cooldown_);
};

struct cannon {
private:
    cannon_type m_type;
    int m_time_left;
    int m_x;
    int m_y;

public:
    cannon(int type, int x, int y);

    void update(unit &enemy, int dt);
    int type();
    int x();
    int y();

    static cannon_stats get_stats(cannon_type type);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CANNON_H
