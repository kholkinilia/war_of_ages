#include "../include/bullet.h"
#include <algorithm>
#include <cassert>
#include "../include/game_constants.h"

namespace war_of_ages {

bullet::bullet(bullet_type type, const vec2f &start, const vec2f &target) noexcept
    : m_type{type}, m_pos{start}, m_dir{target.normalize()} {
    auto stats = bullet::get_stats(m_type);
    m_speed = stats.speed, m_damage = stats.damage;
}

void bullet::update(std::deque<unit> &enemies, float dt) {
    assert(m_is_alive);  // otherwise, it must be deleted by player::clear_dead_objects()
    m_pos += m_dir * m_speed * dt;
    if (m_pos.y <= 0) {
        m_is_alive = false;
        return;
    }
    auto enemy = std::find_if(enemies.rbegin(), enemies.rend(),
                              [this](const unit &u) { return u.is_in(m_pos.x, m_pos.y); });
    if (enemy != enemies.rend()) {
        enemy->decrease_hp(m_damage);
        m_is_alive = false;
    }
}

// Getters

[[nodiscard]] bullet_type bullet::type() const noexcept {
    return m_type;
}

[[nodiscard]] vec2f bullet::pos() const noexcept {
    return m_pos;
}

[[nodiscard]] vec2f bullet::dir() const noexcept {
    return m_dir;
}

[[nodiscard]] bool bullet::is_alive() const noexcept {
    return m_is_alive;
}

[[nodiscard]] bullet_stats bullet::get_stats(bullet_type type) {
    const static bullet_stats stats[NUM_OF_CANNONS + NUM_OF_AGES] = {
        {10, 400, {50, 50}},   {20, 400, {50, 50}},   {30, 400, {50, 50}},   {60, 400, {50, 50}},
        {120, 400, {50, 50}},  {180, 400, {50, 50}},  {360, 400, {50, 50}},  {720, 400, {50, 50}},
        {1080, 400, {50, 50}}, {2060, 400, {50, 50}}, {4120, 400, {50, 50}}, {6180, 400, {50, 50}},
        {50, 250, {50, 50}},   {600, 250, {50, 50}},  {5400, 250, {50, 50}}, {30900, 250, {50, 50}},
    };
    return stats[static_cast<int>(type)];
}
}  // namespace war_of_ages