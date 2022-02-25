#include "../include/cannon.h"
#include <optional>
#include <unordered_map>
#include "../include/bullet.h"
#include "../include/game_constants.h"

namespace war_of_ages {

cannon_stats cannon::get_stats(cannon_type type) {
    const static std::unordered_map<cannon_type, cannon_stats> stats{
        {cannon_type::STONE_LEVEL_1, cannon_stats(500, 2000, 300, 25, 0.1)},
        {cannon_type::STONE_LEVEL_2, cannon_stats(1000, 1750, 400, 30, 0.3)},
        {cannon_type::STONE_LEVEL_3, cannon_stats(3000, 1500, 500, 40, 0.5)}};
    return stats.at(type);
}

cannon_stats::cannon_stats(int cost_,
                           int cooldown_ms_,
                           int attack_radius_pxls_,
                           int damage_,
                           double bullet_speed_)
    : cost(cost_),
      cooldown_ms(cooldown_ms_),
      attack_radius_pxls(attack_radius_pxls_),
      damage(damage_),
      bullet_speed(bullet_speed_) {
}

cannon::cannon(cannon_type type, int x, int y) : m_type(type), m_x(x), m_y(y) {
}

std::optional<bullet> cannon::update(unit &enemy, int dt) noexcept {
    m_time_left -= dt;
    int dist = FIELD_LENGTH_PXLS - enemy.position() - 1 - m_x;
    if (dist <= get_stats(m_type).attack_radius_pxls && m_time_left <= 0) {
        m_time_left += get_stats(m_type).cooldown_ms;
        return bullet(get_stats(m_type).bullet_speed, get_stats(m_type).damage, m_x, m_y, enemy.position(),
                      unit::get_stats(enemy.type()).height_pxls / 2);
    }
    m_time_left = std::max(0, m_time_left);
    return {};
}

cannon_type cannon::type() const noexcept {
    return m_type;
}

int cannon::x() const noexcept {
    return m_x;
}

int cannon::y() const noexcept {
    return m_y;
}

}  // namespace war_of_ages
