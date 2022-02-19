#ifndef WAR_OF_AGES_BULLET_H
#define WAR_OF_AGES_BULLET_H

#include <deque>
#include "age.h"
#include "cannon.h"
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
    bullet(cannon_type type, int x_start, int y_start, int x_target, int y_target) noexcept;
    bullet(age_type age_, int x_start, int y_start, int x_target, int y_target) noexcept;

    void update(const std::deque<unit> &enemies, int dt);

    // Getters

    [[nodiscard]] bullet_type type() const;
    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] int vx() const;
    [[nodiscard]] int vy() const;
    [[nodiscard]] bool is_alive() const;

    [[nodiscard]] static bullet_stats get_stats(bullet_type type);

private:
    bullet_type m_type;
    int m_x, m_y;
    int m_vx, m_vy;
    bool m_is_alive = true;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BULLET_H
