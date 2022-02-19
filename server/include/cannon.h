#ifndef WAR_OF_AGES_CANNON_H
#define WAR_OF_AGES_CANNON_H

#include "unit.h"

namespace war_of_ages {

struct cannon_stats {
    int cost;
    int cooldown;
};

cannon_stats get_cannon_stats(int type);

struct cannon {
private:
    int m_type;
    int m_time_left;
    int m_x;
    int m_y;

public:
    cannon(int type, int x, int y);

    void update(unit &enemy, int dt);
    int type();
    int x();
    int y();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CANNON_H
