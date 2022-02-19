#include "../include/bullet.h"
#include "../include/game_constants.h"
#include "cmath"

namespace war_of_ages {

bullet::bullet(cannon_type type, int x_start, int y_start, int x_target, int y_target) noexcept
    : m_type(static_cast<bullet_type>(type)),
      m_x(x_start),
      m_y(y_start),
      m_vx{x_target - x_start},
      m_vy{y_target - y_start} {
    double scale = sqrt(pow(m_vx, 2) + pow(m_vy, 2));
    m_vx /= scale, m_vy /= scale;
}

bullet::bullet(age_type age_ult, int x_start, int y_start, int x_target, int y_target) noexcept
    : m_type(static_cast<bullet_type>(NUM_OF_CANNONS + static_cast<int>(age_ult))),
      m_x(x_start),
      m_y(y_start),
      m_vx{x_target - x_start},
      m_vy{y_target - y_start} {
    double scale = sqrt(pow(m_vx, 2) + pow(m_vy, 2));
    m_vx /= scale, m_vy /= scale;
}

void bullet::update(const std::deque<unit> &enemies, int dt) {
    // TODO: implement
}

// Getters

[[nodiscard]] bullet_type bullet::type() const {
    return m_type;
}

[[nodiscard]] int bullet::x() const {
    return m_x;
}

[[nodiscard]] int bullet::y() const {
    return m_y;
}

[[nodiscard]] int bullet::vx() const {
    return m_vx;
}

[[nodiscard]] int bullet::vy() const {
    return m_vy;
}

[[nodiscard]] bool bullet::is_alive() const {
    return m_is_alive;
}

[[nodiscard]] bullet_stats bullet::get_stats(bullet_type type) {
    return {};
    // TODO: return stats
}

}  // namespace war_of_ages