#include "../include/player.h"
#include <cassert>

namespace war_of_ages {

// Getters
// TODO: use mutexes

[[nodiscard]] int player::age() const {
    return m_age;
}

[[nodiscard]] int player::exp() const {
    return m_exp;
}

[[nodiscard]] int player::money() const {
    return m_money;
}

[[nodiscard]] std::deque<unit> player::units() const {
    return m_units;
}

[[nodiscard]] std::vector<bullet> player::bullets() const {
    return m_bullets;
}

[[nodiscard]] std::vector<cannon> player::cannons() const {
    return m_cannons;
}

[[nodiscard]] std::deque<int> player::units_to_train() const {
    return m_units_to_train;
}

[[nodiscard]] bool player::is_alive() const {
    assert(!m_units.empty());
    return m_units.front().is_alive();
}
}  // namespace war_of_ages
