#ifndef WAR_OF_AGES_BULLET_H
#define WAR_OF_AGES_BULLET_H

#include <deque>
#include "age.h"
#include "unit.h"

namespace war_of_ages {
enum class bullet_type {
    STONE_LEVEL_1,
    STONE_LEVEL_2,
    STONE_LEVEL_3,
    STONE_ULT,
};

struct bullet_stats {
    int speed;
    int damage;
};

struct bullet {
    bullet(bullet_type type, int x_start, int y_start, int x_target, int y_target);

    void update(std::deque<unit> &enemies, double dt);

    // Getters

    [[nodiscard]] bullet_type type() const noexcept;
    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;
    [[nodiscard]] int vx() const noexcept;
    [[nodiscard]] int vy() const noexcept;
    [[nodiscard]] bool is_alive() const noexcept;

    [[nodiscard]] static bullet_stats get_stats(bullet_type type);

private:
    bullet_type m_type;
    int m_speed;
    int m_damage;
    int m_x, m_y;
    int m_vx, m_vy;
    bool m_is_alive = true;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BULLET_H
