#include "../include/bullet.h"
#include "../include/game_constants.h"
#include "cmath"
#include <cassert>

namespace war_of_ages {

bullet::bullet(bullet_type type, int speed, int damage, int x_start, int y_start, int x_target, int y_target)
    : m_type{type},
      m_speed{speed},
      m_damage{damage},
      m_x{x_start},
      m_y{y_start},
      m_vx{x_target - x_start},
      m_vy{y_target - y_start} {
    double scale = sqrt(pow(m_vx, 2) + pow(m_vy, 2));
    m_vx = static_cast<int>(m_vx / scale), m_vy =  static_cast<int>(m_vy / scale);
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
}  // namespace war_of_ages