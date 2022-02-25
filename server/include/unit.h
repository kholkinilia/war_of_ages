#ifndef WAR_OF_AGES_UNIT_H
#define WAR_OF_AGES_UNIT_H

#include <map>
#include <optional>

namespace war_of_ages {

enum class unit_type { PEASANT, ARCHER, KNIGHT };

struct unit_stats {
    int initial_hp;
    int attack_cooldown_ms;
    int attack_radius_pxls;
    int damage;
    int cost;
    int width_pxls;
    int height_pxls;
    double speed;  // TODO: think of making speed an integer
    unit_stats(int initial_hp_,
               int attack_cooldown_ms_,
               int attack_radius_pxls_,
               int damage_,
               int cost_,
               int width_,
               int height_,
               double speed_);
};

struct unit {
private:
    unit_type m_type;
    int m_remaining_hp;
    int m_time_left = 0;
    int m_position = 0;

public:
    explicit unit(unit_type type);

    void update(unit &enemy, const std::optional<unit> &next_alied_unit, int dt) noexcept;
    void attack(unit &enemy) noexcept;
    void decrease_hp(int damage) noexcept;
    [[nodiscard]] bool is_alive() const noexcept;
    [[nodiscard]] int is_in(int x, int y) const noexcept;
    [[nodiscard]] int dist(unit &enemy) const noexcept;

    [[nodiscard]] int position() const noexcept;
    [[nodiscard]] int remaining_hp() const noexcept;
    [[nodiscard]] unit_type type() const noexcept;

    static unit_stats get_stats(unit_type type) noexcept;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UNIT_H
