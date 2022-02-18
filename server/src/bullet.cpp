#include "../include/bullet.h"
#include "cmath"

namespace war_of_ages {
/*bullet::bullet(int type, int x_start, int y_start, int x_target, int y_target) noexcept
    : m_type(type), m_x(x_start), m_y(y_start) {
    // TODO: think how to normalize m_vx and m_vy
}*/

// Getters
// TODO: use mutexes

[[nodiscard]] int bullet::type() const {
    return m_type;
}

[[nodiscard]] int bullet::x() const {
    return m_x;
}

[[nodiscard]] int bullet::y() const {
    return m_y;
}

[[nodiscard]] bool bullet::is_alive() const {
    return m_is_alive;
}

}  // namespace war_of_ages