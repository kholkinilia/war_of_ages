#include "../include/player.h"
#include <algorithm>
#include <cassert>

namespace war_of_ages {

void player::update(player &enemy, int time) {
    // FIXME: take scoped_lock and update properly

    //    for (auto &unit_ : m_units) {
    //        unit_.update(enemies.back(), /* FIXME: pass dt */ 0);
    //    }
    //    for (auto &bullet_ : m_bullets) {
    //        bullet_.update(enemies, /* FIXME: pass dt */ 0);
    //    }
    //    for (auto &cannon_ : m_cannons) {
    //        cannon_.update(enemies.back(), /* FIXME: pass dt */ 0);
    //    }
}

void player::buy_unit(int unit_level) {
    assert(0 <= unit_level && unit_level < UNITS_PER_AGE);
    std::unique_lock l(m_mutex);
    assert(m_units_to_train.size() < UNITS_QUEUE_SIZE);
    auto type = static_cast<unit_type>(static_cast<int>(m_age) * UNITS_PER_AGE + unit_level);
    int cost = unit::get_stats(type).cost;
    assert(m_money >= cost);
    m_money -= cost;
    m_units_to_train.emplace_back(type);
}

void player::buy_cannon(int cannon_level, int slot) {
    assert(0 <= cannon_level && cannon_level < CANNONS_PER_AGE);
    std::unique_lock l(m_mutex);
    assert(0 <= slot && slot < m_cannons.size());
    auto type = static_cast<cannon_type>(static_cast<int>(m_age) * CANNONS_PER_AGE + cannon_level);
    int cost = cannon::get_stats(type).cost;
    assert(m_money >= cost);
    m_money -= cost;
    m_cannons[cannon_level] =
        cannon{type, CANNONS_SLOTS_COORD_X[cannon_level], CANNONS_SLOTS_COORD_Y[cannon_level]};
}

void player::buy_cannon_slot() {
    std::unique_lock l(m_mutex);
    assert(m_cannons.size() < MAX_CANNON_SLOTS);
    int cost = CANNONS_SLOTS_COSTS[m_cannons.size()];
    assert(m_money >= cost);
    m_money -= cost;
    m_cannons.emplace_back(cannon_type::NONE, CANNONS_SLOTS_COORD_X[m_cannons.size()],
                           CANNONS_SLOTS_COORD_Y[m_cannons.size()]);
}

void player::sell_cannon(int slot) {
    std::unique_lock l(m_mutex);
    assert(m_cannons.at(slot).type() != cannon_type::NONE);
    m_money += cannon::get_stats(m_cannons[slot].type()).cost;
    m_cannons[slot] = cannon{cannon_type::NONE, CANNONS_SLOTS_COORD_X[m_cannons.size()],
                             CANNONS_SLOTS_COORD_Y[m_cannons.size()]};
}

void player::use_ult() {
    // TODO: implement
}

void player::clear_dead_objects() {
    std::unique_lock l(m_mutex);
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
                                   [](const bullet &bullet_) { return !bullet_.is_alive(); }),
                    m_bullets.end());
    m_units.erase(
        std::remove_if(m_units.begin(), m_units.end(), [](const unit &unit_) { return !unit_.is_alive(); }),
        m_units.end());
}

// Getters

[[nodiscard]] age_type player::age() const {
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

[[nodiscard]] std::deque<unit> player::units_to_train() const {
    std::unique_lock l(m_mutex);
    return m_units_to_train;
}

[[nodiscard]] bool player::is_alive() const {
    std::unique_lock l(m_mutex);
    assert(!m_units.empty());
    return m_units.front().is_alive();
}
}  // namespace war_of_ages
