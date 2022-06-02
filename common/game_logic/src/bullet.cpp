#include "../include/bullet.h"
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include "../include/game_constants.h"

namespace war_of_ages {

// TODO: maybe move this function somewhere else?
[[nodiscard]] static bool detect_collision(vec2f pos1, vec2f size1, vec2f pos2, vec2f size2) noexcept {
    /// AABB - AABB collision
    bool collision_x = pos1.x >= pos2.x && pos1.x - size1.x <= pos2.x + size2.x;
    bool collision_y = pos1.y + size1.y >= pos2.y && pos2.y + size2.y >= pos1.y;
    return collision_x && collision_y;
}

bullet_stats::bullet_stats(int damage_, float speed_, vec2f size_) noexcept
    : damage(damage_), speed(speed_), size(size_) {
}

bullet::bullet(bullet_type type, const vec2f &start, const vec2f &target) noexcept
    : m_type{type}, m_pos{start}, m_dir{vec2f{target - start}.normalize()} {
}

void bullet::update(std::deque<unit> &enemies, float dt) {
    assert(m_is_alive);  // otherwise, it must be deleted by player::clear_dead_objects()
    m_pos += m_dir * speed() * dt;
    if (m_pos.y < 0) {
        m_is_alive = false;
        return;
    }
    auto enemy = std::find_if(enemies.rbegin(), enemies.rend(), [this](const unit &u) {
        return detect_collision(m_pos, stats().size, {FIELD_LENGTH_PXLS - u.position(), 0.0f},
                                u.stats().size);
    });
    if (enemy != enemies.rend()) {
        enemy->decrease_hp(damage());
        m_is_alive = false;
    }
}

// Getters

bullet_type bullet::type() const noexcept {
    return m_type;
}

vec2f bullet::pos() const noexcept {
    return m_pos;
}

vec2f bullet::dir() const noexcept {
    return m_dir;
}

bool bullet::is_alive() const noexcept {
    return m_is_alive;
}

const bullet_stats &bullet::stats() const noexcept {
    return get_stats(m_type);
}

const bullet_stats &bullet::get_stats(bullet_type type) {
    const static std::unordered_map<bullet_type, bullet_stats> stats = {
        {bullet_type::STONE_LEVEL_1, {10, 600, {40, 40}}},
        {bullet_type::STONE_LEVEL_2, {20, 700, {30, 30}}},
        {bullet_type::STONE_LEVEL_3, {30, 800, {50, 50}}},
        {bullet_type::CASTLE_LEVEL_1, {60, 800, {30, 30}}},
        {bullet_type::CASTLE_LEVEL_2, {120, 1000, {20, 20}}},
        {bullet_type::CASTLE_LEVEL_3, {180, 1250, {50, 50}}},
        //        {bullet_type::RENAISSANCE_LEVEL_1, {360, 400, {50, 50}}},
        //        {bullet_type::RENAISSANCE_LEVEL_2, {720, 400, {50, 50}}},
        //        {bullet_type::RENAISSANCE_LEVEL_3, {1080, 400, {50, 50}}},
        //        {bullet_type::MODERN_LEVEL_1, {2060, 400, {50, 50}}},
        //        {bullet_type::MODERN_LEVEL_2, {4120, 400, {50, 50}}},
        //        {bullet_type::MODERN_LEVEL_3, {6180, 400, {50, 50}}},
        //        {bullet_type::FUTURE_LEVEL_1, {12360, 400, {50, 50}}},
        //        {bullet_type::FUTURE_LEVEL_2, {24720, 400, {50, 50}}},
        //        {bullet_type::FUTURE_LEVEL_3, {37080, 400, {50, 50}}},
        {bullet_type::STONE_ULT, {50, 1000, {70, 90}}},
        {bullet_type::CASTLE_ULT, {300, 1000, {30, 100}}},
        //        {bullet_type::RENAISSANCE_ULT, {5400, 250, {50, 50}}},
        //        {bullet_type::MODERN_ULT, {30900, 250, {50, 50}}},
        //        {bullet_type::FUTURE_ULT, {185400, 250, {50, 50}}}
        {bullet_type::NONE, {0, 0, {0, 0}}}};
    return stats.at(type);
}

int bullet::damage() const noexcept {
    return stats().damage;
}

float bullet::speed() const noexcept {
    return stats().speed;
}
}  // namespace war_of_ages
