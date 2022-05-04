#include "../include/unit.h"
#include <algorithm>
#include <optional>
#include <unordered_map>
#include "../include/game_constants.h"

namespace war_of_ages {

unit_stats::unit_stats(float time_to_train_s_,
                       int initial_hp_,
                       float attack_duration_s_,
                       float attack_time_s_,
                       float attack_radius_pxls_,
                       int damage_,
                       int cost_,
                       vec2f size_,
                       float speed_) noexcept
    : time_to_train_s(time_to_train_s_),
      initial_hp(initial_hp_),
      attack_duration_s(attack_duration_s_),
      attack_time_s(attack_time_s_),
      attack_radius_pxls(attack_radius_pxls_),
      damage(damage_),
      cost(cost_),
      size(size_),
      speed(speed_) {
}

const unit_stats &unit::get_stats(unit_type type) noexcept {
    const static std::unordered_map<unit_type, unit_stats> stats{
        {unit_type::PEASANT, unit_stats(2, 50, 0.5, 0.5, 0, 25, 25, {100, 200}, 100)},
        {unit_type::ARCHER, unit_stats(3, 70, 1, 1, 500, 30, 50, {100, 200}, 100)},
        {unit_type::CHARIOT, unit_stats(4, 100, 1.5, 1.25, 0, 40, 100, {200, 160}, 150)},
        {unit_type::STONE_TOWER, unit_stats(0, 5000, 0, 0, 0, 0, 0, {0, 0}, 0)}};
    return stats.at(type);
}

unit::unit(unit_type type) noexcept : m_type(type), m_remaining_hp(get_stats(type).initial_hp) {
}

void unit::attack(unit &enemy) noexcept {
    enemy.decrease_hp(stats().damage);
    m_attack_progress_s = 0;
}

void unit::decrease_hp(int damage) noexcept {
    m_remaining_hp -= damage;
}

bool unit::is_alive() const noexcept {
    return m_remaining_hp > 0;
}

void unit::update(unit &enemy, const std::optional<unit> &next_allied_unit, float dt) noexcept {
    m_lifetime += dt;
    m_attacking = m_walking = false;
    if (stats().attack_radius_pxls >= dist(enemy)) {
        m_attacking = true;
        m_attack_progress_s += dt;
        if (m_attack_progress_s - dt <= stats().attack_time_s &&
            stats().attack_time_s <= m_attack_progress_s) {
            attack(enemy);
        }
        if (m_attack_progress_s >= stats().attack_duration_s) {
            m_attack_progress_s -= stats().attack_duration_s;
        }
    } else {
        m_attack_progress_s = 0;
    }
    if (!next_allied_unit) {
        move(dt, FIELD_LENGTH_PXLS - enemy.position());
    } else {
        move(dt, next_allied_unit->position() - next_allied_unit->stats().size.x);
    }
}

float unit::position() const noexcept {
    return m_position;
}

int unit::remaining_hp() const noexcept {
    return m_remaining_hp;
}

float unit::dist(unit &enemy) const noexcept {
    return std::max(0.f, (FIELD_LENGTH_PXLS - enemy.position()) - position());
}

unit_type unit::type() const noexcept {
    return m_type;
}

void unit::move(float dt, float bound_position) {
    float dist = bound_position - m_position;
    if (dist <= 0) {
        m_walking_time = 0;
        return;
    }
    m_walking = true;
    m_walking_time += dt;
    m_position += std::min(dt * stats().speed, dist);
}

const unit_stats &unit::stats() const noexcept {
    return get_stats(m_type);
}

bool unit::is_walking() const noexcept {
    return m_walking;
}

bool unit::is_attacking() const noexcept {
    return m_attacking;
}

float unit::walking_time() const noexcept {
    return m_walking_time;
}

float unit::attack_progress() const noexcept {
    return m_attack_progress_s;
}

}  // namespace war_of_ages
