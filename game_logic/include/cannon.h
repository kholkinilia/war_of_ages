#ifndef WAR_OF_AGES_CANNON_H
#define WAR_OF_AGES_CANNON_H

#include <optional>
#include "bullet.h"
#include "unit.h"

namespace war_of_ages {

enum class cannon_type { STONE_LEVEL_1, STONE_LEVEL_2, STONE_LEVEL_3, NONE };

struct cannon_stats {
    bullet_type b_type;
    int cost;
    double cooldown_s;
    int attack_radius_pxls;
};

struct cannon {
private:
    cannon_type m_type;
    double m_time_left_to_attack = 0;
    int m_x;
    int m_y;

public:
    cannon(cannon_type type, int x, int y);

    std::optional<bullet> update(unit &enemy, double dt) noexcept;
    [[nodiscard]] cannon_type type() const noexcept;
    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;

    static cannon_stats get_stats(cannon_type type);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CANNON_H
