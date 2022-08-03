#ifndef WAR_OF_AGES_BULLET_H
#define WAR_OF_AGES_BULLET_H

#include <deque>
#include <vector>
#include "age.h"
#include "unit.h"
#include "vec2f.h"

namespace war_of_ages {

enum class bullet_type : std::int16_t {
    STONE_LEVEL_1,
    STONE_LEVEL_2,
    STONE_LEVEL_3,
    CASTLE_LEVEL_1,
    CASTLE_LEVEL_2,
    CASTLE_LEVEL_3,
    //    RENAISSANCE_LEVEL_1,
    //    RENAISSANCE_LEVEL_2,
    //    RENAISSANCE_LEVEL_3,
    //    MODERN_LEVEL_1,
    //    MODERN_LEVEL_2,
    //    MODERN_LEVEL_3,
    //    FUTURE_LEVEL_1,
    //    FUTURE_LEVEL_2,
    //    FUTURE_LEVEL_3,
    STONE_ULT,
    CASTLE_ULT,
    //    RENAISSANCE_ULT,
    //    MODERN_ULT,
    //    FUTURE_ULT,,
    NONE
};

const inline std::vector<bullet_type> ult_bullet_types { bullet_type::STONE_ULT, bullet_type::CASTLE_ULT };

struct bullet_snapshot {
    bullet_type type;
    float x_start;
    float y_start;
    float x_target;
    float y_target;
};

struct bullet_stats {
    int damage;
    float speed;
    vec2f size;
    bullet_stats(int damage_, float speed_, vec2f size_) noexcept;
};

struct bullet {
    bullet(bullet_type type, const vec2f &start, const vec2f &target) noexcept;
    bullet() = default;

    void update(std::deque<std::shared_ptr<unit>> &enemies, float dt);

    // Getters

    [[nodiscard]] bullet_type type() const noexcept;
    [[nodiscard]] vec2f pos() const noexcept;
    [[nodiscard]] vec2f dir() const noexcept;
    [[nodiscard]] bool is_alive() const noexcept;
    [[nodiscard]] const bullet_stats &stats() const noexcept;
    [[nodiscard]] bullet_snapshot get_snapshot() const noexcept;

    [[nodiscard]] static const bullet_stats &get_stats(bullet_type type);

    virtual void post_create_action(std::string owner_handle) const = 0;
    virtual void post_collision_action() const = 0;

protected:
    [[nodiscard]] int damage() const noexcept;
    [[nodiscard]] float speed() const noexcept;

    bullet_type m_type = bullet_type::STONE_ULT;
    vec2f m_pos;
    vec2f m_dir;
    bool m_is_alive = true;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_BULLET_H
