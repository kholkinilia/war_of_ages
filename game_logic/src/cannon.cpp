#include "../include/cannon.h"
#include <optional>
#include <unordered_map>
#include "../include/bullet.h"
#include "../include/game_constants.h"

namespace war_of_ages {

cannon_stats cannon::get_stats(cannon_type type) {
    const static std::unordered_map<cannon_type, cannon_stats> stats{
        {cannon_type::STONE_LEVEL_1, cannon_stats(bullet_type::STONE_LEVEL_1, 500, 2, 700)},
        {cannon_type::STONE_LEVEL_2, cannon_stats(bullet_type::STONE_LEVEL_2, 1000, 1.75, 1000)},
        {cannon_type::STONE_LEVEL_3, cannon_stats(bullet_type::STONE_LEVEL_3, 3000, 1.5, 1200)}};
    return stats.at(type);
}

cannon_stats::cannon_stats(bullet_type b_type_, int cost_, double cooldown_s_, int attack_radius_pxls_)
    : b_type(b_type_), cost(cost_), cooldown_s(cooldown_s_), attack_radius_pxls(attack_radius_pxls_) {
}

cannon::cannon(cannon_type type, int x, int y) : m_type(type), m_x(x), m_y(y) {
}

std::optional<bullet> cannon::update(unit &enemy, double dt) noexcept {
    m_time_left_to_attack -= dt;
    int dist = FIELD_LENGTH_PXLS - enemy.position() - 1 - m_x;
    if (dist <= get_stats(m_type).attack_radius_pxls && m_time_left_to_attack <= 0) {
        m_time_left_to_attack = get_stats(m_type).cooldown_s;
        return bullet(cannon::get_stats(m_type).b_type, m_x, m_y, FIELD_LENGTH_PXLS - enemy.position() - 1 - m_x,
                      unit::get_stats(enemy.type()).height_pxls / 2);
    }
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
