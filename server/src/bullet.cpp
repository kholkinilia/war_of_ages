#include "../include/bullet.h"
#include "cmath"

namespace war_of_ages {

// FIXME: think how to normalize m_vx and m_vy
bullet::bullet(int type, int x_start, int y_start, int x_target, int y_target) noexcept
    : m_type(type), m_x(x_start), m_y(y_start), m_vx{}, m_vy{} {
}

void bullet::update(std::deque<unit> &enemies, int dt) {
    // TODO: implement
}

// Getters

[[nodiscard]] int bullet::type() const {
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

[[nodiscard]] bullet_stats bullet::get_stats(int bullet_type) {
    return {};
    // TODO: return stats
}

}  // namespace war_of_ages