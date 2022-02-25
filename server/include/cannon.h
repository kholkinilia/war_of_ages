#ifndef WAR_OF_AGES_CANNON_H
#define WAR_OF_AGES_CANNON_H

#include <optional>
#include "bullet.h"
#include "unit.h"

namespace war_of_ages {

enum class cannon_type { STONE_LEVEL_1, STONE_LEVEL_2, STONE_LEVEL_3, NONE };  // STONE -- age

struct cannon_stats {
    int cost;
    int cooldown_ms;
    int attack_radius_pxls;
    int damage;
    double bullet_speed;  // TODO: think of making speed an integer
    cannon_stats(int cost_, int cooldown_ms_, int attack_radius_pxls_, int damage_, double bullet_speed_);
};

struct cannon {
private:
    cannon_type m_type;
    int m_time_left = 0;
    int m_x;
    int m_y;

public:
    cannon(cannon_type type, int x, int y);

    std::optional<bullet> update(unit &enemy, int dt) noexcept;
    [[nodiscard]] cannon_type type() const noexcept;
    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;

    static cannon_stats get_stats(cannon_type type);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CANNON_H
