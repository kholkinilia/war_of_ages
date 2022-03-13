#ifndef WAR_OF_AGES_BULLET_H
#define WAR_OF_AGES_BULLET_H

#include <deque>
#include "age.h"
#include "unit.h"
#include "vec2f.h"

namespace war_of_ages {
enum class bullet_type {
    STONE_LEVEL_1,
    STONE_LEVEL_2,
    STONE_LEVEL_3,
    CASTLE_LEVEL_1,
    CASTLE_LEVEL_2,
    CASTLE_LEVEL_3,
    RENAISSANCE_LEVEL_1,
    RENAISSANCE_LEVEL_2,
    RENAISSANCE_LEVEL_3,
    MODERN_LEVEL_1,
    MODERN_LEVEL_2,
    MODERN_LEVEL_3,
    FUTURE_LEVEL_1,
    FUTURE_LEVEL_2,
    FUTURE_LEVEL_3,
    STONE_ULT,
    CASTLE_ULT,
    RENAISSANCE_ULT,
    MODERN_ULT,
    FUTURE_ULT,
};

struct bullet_stats {
    int damage;
    float speed;
    vec2f size;
    bullet_stats(int damage_, float speed_, vec2f size_) : damage(damage_), speed(speed_), size(size_) {
    }
};

struct bullet {
    bullet(bullet_type type, const vec2f &start, const vec2f &target) noexcept;

    void update(std::deque<unit> &enemies, float dt);

    // Getters

    [[nodiscard]] bullet_type type() const noexcept;
    [[nodiscard]] vec2f pos() const noexcept;
    [[nodiscard]] vec2f dir() const noexcept;
    [[nodiscard]] bool is_alive() const noexcept;
    [[nodiscard]] const bullet_stats &stats() const noexcept;

    [[nodiscard]] static const bullet_stats &get_stats(bullet_type type);

private:
    [[nodiscard]] int damage() const noexcept;
    [[nodiscard]] float speed() const noexcept;

    bullet_type m_type;
    vec2f m_pos;
    vec2f m_dir;
    bool m_is_alive = true;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BULLET_H
