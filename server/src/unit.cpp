#include "../include/unit.h"
#include <map>
namespace war_of_ages {

unit_stats::unit_stats(int initial_hp_,
                       int attack_cooldown_ms_,
                       int attack_radius_pxls_,
                       int damage_,
                       int cost_)
    : initial_hp(initial_hp_),
      attack_cooldown_ms(attack_cooldown_ms_),
      attack_radius_pxls(attack_radius_pxls_),
      damage(damage_),
      cost(cost_) {
}

unit_stats get_unit_stats(unit_type type) {
    const static std::map<unit_type, unit_stats> stats{
        {unit_type::PEASANT, unit_stats(50, 500, 0, 25, 25)},
        {unit_type::ARCHER, unit_stats(70, 1000, 100, 30, 50)},
        {unit_type::KNIGHT, unit_stats(100, 1500, 0, 40, 100)}};
    return stats.at(type);
}

unit::unit(unit_type type) : m_type(type) {
}

void unit::attack(unit &enemy) const {
    enemy.decrease_hp(get_unit_stats(m_type).damage);
}

void unit::decrease_hp(int damage) {
    m_remaining_hp -= damage;
}

bool unit::is_alive() const {
    return m_remaining_hp >= 0;
}

void unit::update(unit &enemy, int dt) {
    // TODO: implement
}

}  // namespace war_of_ages