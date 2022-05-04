#include "../include/cannon.h"
#include <optional>
#include <unordered_map>
#include "../include/bullet.h"
#include "../include/game_constants.h"

namespace war_of_ages {

const cannon_stats &cannon::get_stats(cannon_type type) noexcept {
    const static std::unordered_map<cannon_type, cannon_stats> stats{
        {cannon_type::STONE_LEVEL_1, cannon_stats{bullet_type::STONE_LEVEL_1, 500, 2, 2, 700}},
        {cannon_type::STONE_LEVEL_2, cannon_stats{bullet_type::STONE_LEVEL_2, 1000, 1.75, 1.75, 1000}},
        {cannon_type::STONE_LEVEL_3, cannon_stats{bullet_type::STONE_LEVEL_3, 3000, 1.5, 1.5, 1200}},
        {cannon_type::NONE, cannon_stats{bullet_type::STONE_LEVEL_1, 0, 0, 0, 0}}};
    return stats.at(type);
}

cannon::cannon(cannon_type type, vec2f cannon_position) noexcept
    : m_type(type),
      m_muzzle_position(vec2f{cannon_position.x, cannon_position.y + static_cast<float>(CANNON_HEIGHT) / 2}) {
}

std::optional<bullet> cannon::update(unit &enemy, float dt) noexcept {
    if (m_type == cannon_type::NONE) {
        return std::nullopt;
    }
    float dist = FIELD_LENGTH_PXLS - enemy.position() - m_muzzle_position.x;
    if (dist <= get_stats(m_type).attack_radius_pxls) {
        m_attack_progress_s += dt;
        if (m_attack_progress_s - dt <= stats().attack_time_s &&
            stats().attack_time_s <= m_attack_progress_s) {
            if (m_attack_progress_s >= stats().attack_duration_s) {
                m_attack_progress_s -= stats().attack_duration_s;
            }
            return bullet(stats().b_type, m_muzzle_position,
                          {FIELD_LENGTH_PXLS - enemy.position(), unit::get_stats(enemy.type()).size.y / 2});
        }
        if (m_attack_progress_s >= stats().attack_duration_s) {
            m_attack_progress_s -= stats().attack_duration_s;
        }
    } else {
        m_attack_progress_s = 0;
    }
    return std::nullopt;
}

cannon_type cannon::type() const noexcept {
    return m_type;
}

vec2f cannon::muzzle_position() const noexcept {
    return m_muzzle_position;
}

const cannon_stats &cannon::stats() const noexcept {
    return get_stats(m_type);
}

float cannon::attack_progress() const noexcept {
    return m_attack_progress_s;
}

}  // namespace war_of_ages
