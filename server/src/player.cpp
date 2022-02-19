#include "../include/player.h"
#include <cassert>

namespace war_of_ages {

player::player() noexcept {
    // TODO: implement;
}

void player::update(std::vector<unit> &enemies, int time) {
    // TODO: implement
}

void player::buy_unit(int unit_level) {
    // TODO: implement
}

void player::buy_cannon(int cannon_type) {
    // TODO: implement
}

void player::buy_cannon_slot() {
    std::unique_lock l(m_mutex);
    assert(m_cannons.size() < MAX_CANNON_SLOTS);
    // TODO: implement
}

void player::sell_cannon(int slot) {
    std::unique_lock l(m_mutex);
    assert(0 < slot && slot < m_cannons.size());
    auto &cann = m_cannons[slot];
    // TODO: implement
}

void player::use_ult() {
    // TODO: implement
}

void player::clear_dead_objects() {
    // TODO: implement
}

// Getters

[[nodiscard]] int player::age() const {
    std::unique_lock l(m_mutex);
    return m_age;
}

[[nodiscard]] int player::exp() const {
    std::unique_lock l(m_mutex);
    return m_exp;
}

[[nodiscard]] int player::money() const {
    std::unique_lock l(m_mutex);
    return m_money;
}

[[nodiscard]] std::deque<unit> player::units() const {
    std::unique_lock l(m_mutex);
    return m_units;
}

[[nodiscard]] std::vector<bullet> player::bullets() const {
    std::unique_lock l(m_mutex);
    return m_bullets;
}

[[nodiscard]] std::vector<cannon> player::cannons() const {
    std::unique_lock l(m_mutex);
    return m_cannons;
}

[[nodiscard]] std::deque<int> player::units_to_train() const {
    std::unique_lock l(m_mutex);
    return m_units_to_train;
}

[[nodiscard]] bool player::is_alive() const {
    std::unique_lock l(m_mutex);
    assert(!m_units.empty());
    return m_units.front().is_alive();
}
}  // namespace war_of_ages
