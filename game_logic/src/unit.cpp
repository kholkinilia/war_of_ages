#include "../include/unit.h"
#include <algorithm>
#include <optional>
#include <unordered_map>
#include "../include/game_constants.h"

namespace war_of_ages {

unit_stats::unit_stats(float time_to_train_s_,
                       int initial_hp_,
                       float attack_cooldown_s_,
                       float attack_radius_pxls_,
                       int damage_,
                       int cost_,
                       vec2f size_,
                       float speed_)
    : time_to_train_s(time_to_train_s_),
      initial_hp(initial_hp_),
      attack_cooldown_s(attack_cooldown_s_),
      attack_radius_pxls(attack_radius_pxls_),
      damage(damage_),
      cost(cost_),
      size(size_),
      speed(speed_) {
}

unit_stats unit::get_stats(unit_type type) noexcept {
    const static std::unordered_map<unit_type, unit_stats> stats{
        {unit_type::PEASANT, unit_stats(2, 50, 0.5, 0, 25, 25, {100, 200}, 5)},
        {unit_type::ARCHER, unit_stats(3, 70, 1, 100, 30, 50, {100, 200}, 5)},
        {unit_type::CHARIOT, unit_stats(4, 100, 1.5, 0, 40, 100, {200, 160}, 10)},
        {unit_type::STONE_TOWER, unit_stats(0, 5000, 0, 0, 0, 0, {0, 0}, 0)}};
    return stats.at(type);
}

unit::unit(unit_type type) : m_type(type), m_remaining_hp(get_stats(type).initial_hp) {
}

void unit::attack(unit &enemy) noexcept {
    enemy.decrease_hp(unit::get_stats(m_type).damage);
    m_time_left_to_attack = get_stats(m_type).attack_cooldown_s;
}

void unit::decrease_hp(int damage) noexcept {
    m_remaining_hp -= damage;
}

bool unit::is_alive() const noexcept {
    return m_remaining_hp >= 0;
}

void unit::update(unit &enemy, const std::optional<unit> &next_allied_unit, float dt) noexcept {
    if (get_stats(m_type).attack_radius_pxls <= dist(enemy)) {
        m_time_left_to_attack -= dt;
        if (m_time_left_to_attack <= 0) {
            attack(enemy);
        }
    } else if (!next_allied_unit) {
        move(dt, FIELD_LENGTH_PXLS - enemy.position() + 1);
    } else {
        move(dt, next_allied_unit->position() - get_stats(next_allied_unit->type()).size.x + 1);
    }
}

float unit::position() const noexcept {
    return m_position;
}

int unit::remaining_hp() const noexcept {
    return m_remaining_hp;
}

bool unit::is_in(vec2f point) const noexcept {
    return m_position - get_stats(m_type).size.x < point.x && point.x <= m_position && 0 <= point.y &&
           point.y < get_stats(m_type).size.y;
}

float unit::dist(unit &enemy) const noexcept {
    return std::max(0.f, (FIELD_LENGTH_PXLS - enemy.position() - 1) - position());
}

unit_type unit::type() const noexcept {
    return m_type;
}

void unit::move(float dt, float bound_position) {
    float dist = bound_position - m_position;
    if (dist <= 0) {
        return;
    }
    m_position += std::min(dt * get_stats(m_type).speed, dist);
}

const unit_stats &unit::stats() const noexcept {
    return get_stats(m_type);
}

}  // namespace war_of_ages
