#include "../include/bullet.h"
#include <cassert>
#include "../include/game_constants.h"
#include "cmath"

namespace war_of_ages {

bullet::bullet(bullet_type type, int x_start, int y_start, int x_target, int y_target)
    : m_type{type}, m_x{x_start}, m_y{y_start}, m_vx{x_target - x_start}, m_vy{y_target - y_start} {
    double scale = sqrt(pow(m_vx, 2) + pow(m_vy, 2));
    m_vx = static_cast<int>(m_vx / scale), m_vy = static_cast<int>(m_vy / scale);
    auto stats = bullet::get_stats(m_type);
    m_speed = stats.speed, m_damage = stats.damage;
}

void bullet::update(std::deque<unit> &enemies, double dt) {
    assert(m_is_alive);  // must be deleted by player::clear_dead_objects()
    m_x += static_cast<int>(m_vx * m_speed * dt), m_y += static_cast<int>(m_vy * m_speed * dt);
    if (m_vy <= 0) {
        m_is_alive = false;
        return;
    }
    for (auto it = enemies.rbegin(); it != enemies.rend(); ++it) {
        auto &enemy = *it;
        if (enemy.is_in(m_x, m_y)) {
            enemy.decrease_hp(m_damage);
            m_is_alive = false;
            break;
        }
    }
}

// Getters

[[nodiscard]] bullet_type bullet::type() const noexcept {
    return m_type;
}

[[nodiscard]] int bullet::x() const noexcept {
    return m_x;
}

[[nodiscard]] int bullet::y() const noexcept {
    return m_y;
}

[[nodiscard]] int bullet::vx() const noexcept {
    return m_vx;
}

[[nodiscard]] int bullet::vy() const noexcept {
    return m_vy;
}

[[nodiscard]] bool bullet::is_alive() const noexcept {
    return m_is_alive;
}

[[nodiscard]] bullet_stats bullet::get_stats(bullet_type type) {
    constexpr static bullet_stats stats[NUM_OF_CANNONS + NUM_OF_AGES] = {
        {400, 10},   {400, 20},   {400, 30},   {400, 60},   {400, 120}, {400, 180}, {400, 360},  {400, 720},
        {400, 1080}, {400, 2060}, {400, 4120}, {400, 6180}, {250, 50},  {250, 600}, {250, 5400}, {250, 30900},
    };
    return stats[static_cast<int>(type)];
}
}  // namespace war_of_ages