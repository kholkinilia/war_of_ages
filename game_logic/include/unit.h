#ifndef WAR_OF_AGES_UNIT_H
#define WAR_OF_AGES_UNIT_H

#include <optional>
#include "../include/vec2f.h"

namespace war_of_ages {

// TODO: make tower a different struct
enum class unit_type { PEASANT, ARCHER, CHARIOT, STONE_TOWER };

struct unit_stats {
    float time_to_train_s;
    int initial_hp;
    float attack_cooldown_s;
    float attack_radius_pxls;
    int damage;
    int cost;
    vec2f size;
    float speed;
    unit_stats(float time_to_train_s_,
               int initial_hp_,
               float attack_cooldown_s_,
               float attack_radius_pxls_,
               int damage_,
               int cost_,
               vec2f size,
               float speed_);
};

struct unit {
private:
    unit_type m_type;
    int m_remaining_hp;
    float m_time_left_to_attack = 0;
    float m_position = 0;

    void move(float dt, float bound_position);

public:
    explicit unit(unit_type type);

    void update(unit &enemy, const std::optional<unit> &next_allied_unit, float dt) noexcept;
    void attack(unit &enemy) noexcept;
    void decrease_hp(int damage) noexcept;
    [[nodiscard]] bool is_alive() const noexcept;
    [[nodiscard]] bool is_in(vec2f point) const noexcept;
    [[nodiscard]] float dist(unit &enemy) const noexcept;

    [[nodiscard]] float position() const noexcept;
    [[nodiscard]] int remaining_hp() const noexcept;
    [[nodiscard]] unit_type type() const noexcept;

    static unit_stats get_stats(unit_type type) noexcept;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UNIT_H
