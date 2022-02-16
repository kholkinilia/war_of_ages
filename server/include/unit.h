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
    unit_stats(int initial_hp_, int attack_cooldown_ms_, int attack_radius_pxls_, int damage_);
};

unit_stats get_unit_stats(unit_type type);

struct unit {
private:
    unit_type type;
    int remaining_hp;
    int last_beat;
    int position;
public:
    explicit unit(unit_type type_);

    // TODO: void update(player opponent, int direction);
    void attack(unit& enemy);
    void decrease_hp(int damage);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UNIT_H
