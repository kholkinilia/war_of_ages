#ifndef WAR_OF_AGES_BULLET_H
#define WAR_OF_AGES_BULLET_H

#include <queue>
#include "unit.h"

namespace war_of_ages {
struct bullet_stats {
    int velocity;
    int damage;
};

struct bullet {
    bullet(int type, int x_start, int y_start, int x_target, int y_target) noexcept;

    void update(std::queue<unit> &enemies, int dt);

    // Getters

    [[nodiscard]] int type() const;
    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] bool is_alive() const;

private:
    int m_type;
    int m_x, m_y;
    const int m_vx, m_vy;
    bool m_is_alive = true;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BULLET_H
