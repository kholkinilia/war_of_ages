#include "../include/unit.h"
#include <algorithm>
#include <map>
#include <optional>

namespace war_of_ages {

unit_stats::unit_stats(int initial_hp_,
                       int attack_cooldown_ms_,
                       int attack_radius_pxls_,
                       int damage_,
                       int cost_,
                       int width_,
                       int height_,
                       double speed_)
    : initial_hp(initial_hp_),
      attack_cooldown_ms(attack_cooldown_ms_),
      attack_radius_pxls(attack_radius_pxls_),
      damage(damage_),
      cost(cost_),
      width_pxls(width_),
      height_pxls(height_),
      speed(speed_) {
}

unit_stats unit::get_stats(unit_type type) noexcept {
    const static std::map<unit_type, unit_stats> stats{
        {unit_type::PEASANT, unit_stats(50, 500, 0, 25, 25, 50, 100, 0.01)},
        {unit_type::ARCHER, unit_stats(70, 1000, 100, 30, 50, 50, 100, 0.01)},
        {unit_type::KNIGHT, unit_stats(100, 1500, 0, 40, 100, 100, 100, 0.05)}};
    return stats.at(type);
}

unit::unit(unit_type type) : m_type(type), m_remaining_hp(get_stats(type).initial_hp) {
}

void unit::attack(unit &enemy) noexcept {
    enemy.decrease_hp(unit::get_stats(m_type).damage);
    m_time_left += get_stats(m_type).attack_cooldown_ms;
}

void unit::decrease_hp(int damage) noexcept {
    m_remaining_hp -= damage;
}

bool unit::is_alive() const noexcept {
    return m_remaining_hp >= 0;
}

void unit::update(unit &enemy, const std::optional<unit> &next_alied_unit, int dt) noexcept {
    if (get_stats(m_type).attack_radius_pxls <= dist(enemy)) {
        m_time_left -= dt;
        if (m_time_left <= 0) {
            attack(enemy);
        }
    } else if (!next_alied_unit) {
        int dist =
            next_alied_unit->position() - get_stats(next_alied_unit->type()).width_pxls + 1 - m_position;
        if (dist <= 0) {
            return;
        }
        m_position += static_cast<int>(std::min(dt * get_stats(m_type).speed, 1. * dist));
    }
}

int unit::position() const noexcept {
    return m_position;
}

int unit::remaining_hp() const noexcept {
    return m_remaining_hp;
}

int unit::is_in(int x, int y) const noexcept {
    return m_position - get_stats(m_type).width_pxls < x && x <= m_position && 0 <= y &&
           y < get_stats(m_type).height_pxls;
}

int unit::dist(unit &enemy) const noexcept {
    return std::max(0, position() - enemy.position());
}

unit_type unit::type() const noexcept {
    return m_type;
}

}  // namespace war_of_ages
