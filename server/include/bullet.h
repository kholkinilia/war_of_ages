#ifndef WAR_OF_AGES_BULLET_H
#define WAR_OF_AGES_BULLET_H

#include <deque>
#include "unit.h"

namespace war_of_ages {
struct bullet_stats {
    int speed;
    int damage;
};

struct bullet {
    bullet(int type, int x_start, int y_start, int x_target, int y_target) noexcept;

    void update(std::deque<unit> &enemies, int dt);

    // Getters

    [[nodiscard]] int type() const;
    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] int vx() const;
    [[nodiscard]] int vy() const;
    [[nodiscard]] bool is_alive() const;

    [[nodiscard]] static bullet_stats get_stats(int bullet_type);

private:
    int m_type;
    int m_x, m_y;
    int m_vx, m_vy;
    bool m_is_alive = true;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BULLET_H
