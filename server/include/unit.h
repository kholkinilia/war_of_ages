#ifndef WAR_OF_AGES_UNIT_H
#define WAR_OF_AGES_UNIT_H

#include <map>

namespace war_of_ages {

enum class unit_type { PEASANT, ARCHER, KNIGHT };

struct unit_stats {
    int initial_hp;
    int attack_cooldown_ms;
    int attack_radius_pxls;
    int damage;
    int cost;
    int width;
    unit_stats(int initial_hp_,
               int attack_cooldown_ms_,
               int attack_radius_pxls_,
               int damage_,
               int cost_,
               int width_);
};

struct unit {
private:
    const static inline int SPEED = 1;

    unit_type m_type;
    int m_remaining_hp;
    int m_time_left;
    int m_position;

public:
    explicit unit(unit_type type);

    void update(unit &enemy, int dt);
    void attack(unit &enemy) const;
    void decrease_hp(int damage);
    [[nodiscard]] bool is_alive() const;

    [[nodiscard]] int position() const;
    [[nodiscard]] int remaining_hp() const;

    static unit_stats get_stats(unit_type type);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UNIT_H
