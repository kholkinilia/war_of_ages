#include "../include/cannon.h"
#include <optional>
#include <unordered_map>
#include "../include/bullet.h"
#include "../include/game_constants.h"

namespace war_of_ages {

cannon_stats cannon::get_stats(cannon_type type) {
    const static std::unordered_map<cannon_type, cannon_stats> stats{
        {cannon_type::STONE_LEVEL_1, cannon_stats{bullet_type::STONE_LEVEL_1, 500, 2, 700}},
        {cannon_type::STONE_LEVEL_2, cannon_stats{bullet_type::STONE_LEVEL_2, 1000, 1.75, 1000}},
        {cannon_type::STONE_LEVEL_3, cannon_stats{bullet_type::STONE_LEVEL_3, 3000, 1.5, 1200}}};
    return stats.at(type);
}

cannon::cannon(cannon_type type, vec2f muzzle_position) : m_type(type), m_muzzle_position(muzzle_position) {
}

std::optional<bullet> cannon::update(unit &enemy, float dt) noexcept {
    if (m_type == cannon_type::NONE) {
        return {};
    }
    m_time_left_to_attack -= dt;
    float dist = FIELD_LENGTH_PXLS - enemy.position() - 1 - m_muzzle_position.x;
    if (dist <= get_stats(m_type).attack_radius_pxls && m_time_left_to_attack <= 0) {
        m_time_left_to_attack = get_stats(m_type).cooldown_s;
        return bullet(cannon::get_stats(m_type).b_type, m_muzzle_position,
                      {FIELD_LENGTH_PXLS - enemy.position() - 1, unit::get_stats(enemy.type()).size.y / 2});
    }
    return {};
}

cannon_type cannon::type() const noexcept {
    return m_type;
}

vec2f cannon::muzzle_position() const noexcept {
    return m_muzzle_position;
}

}  // namespace war_of_ages
