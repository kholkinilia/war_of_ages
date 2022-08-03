#ifndef WAR_OF_AGES_CANNON_H
#define WAR_OF_AGES_CANNON_H

#include <optional>
#include "bullet.h"
#include "unit.h"

namespace war_of_ages {

enum class cannon_type {
    STONE_LEVEL_1,
    STONE_LEVEL_2,
    STONE_LEVEL_3,
    CASTLE_LEVEL_1,
    CASTLE_LEVEL_2,
    CASTLE_LEVEL_3,
    NONE
};

struct cannon_stats {
    bullet_type b_type;
    int cost;
    float attack_duration_s;
    float attack_time_s;
    float attack_radius_pxls;
};

struct cannon {
private:
    cannon_type m_type = cannon_type::NONE;
    float m_attack_progress_s = 0;
    vec2f m_muzzle_position;

public:
    cannon(cannon_type type, vec2f muzzle_position) noexcept;
    cannon() = default;

    [[nodiscard]] std::optional<bullet> update(std::shared_ptr<unit> enemy, float dt) noexcept;
    [[nodiscard]] cannon_type type() const noexcept;
    [[nodiscard]] const cannon_stats &stats() const noexcept;
    [[nodiscard]] vec2f muzzle_position() const noexcept;
    [[nodiscard]] float attack_progress() const noexcept;

    [[nodiscard]] const static cannon_stats &get_stats(cannon_type type) noexcept;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CANNON_H
