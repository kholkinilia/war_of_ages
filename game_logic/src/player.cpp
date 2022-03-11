#include "../include/player.h"
#include <algorithm>
#include <cassert>

namespace war_of_ages {

void player::update(player &enemy, double dt) {
    std::scoped_lock l(m_mutex, enemy.m_mutex);
    auto &enemies = enemy.m_units;
    for (auto it = m_units.begin() + 1; it != m_units.end(); ++it) {
        auto &unit_ = *it;
        unit_.update(enemies.back(), (it + 1 != m_units.end() ? *(it + 1) : std::optional<unit>{}), dt);
    }
    for (auto &bullet_ : m_bullets) {
        bullet_.update(enemies, dt);
    }
    for (auto &cannon_ : m_cannons) {
        cannon_.update(enemies.back(), dt);
    }
    m_ult_cooldown = std::max(m_ult_cooldown - dt, 0.0);
    m_training_time_left = std::max(m_training_time_left - dt, 0.0);
    if (m_training_time_left == 0.0 && !m_units_to_train.empty()) {
        m_units.push_front(m_units_to_train[0]);
        std::swap(m_units[0], m_units[1]);
        m_units_to_train.pop_front();
        if (!m_units_to_train.empty()) {
            m_training_time_left = unit::get_stats(m_units_to_train[0].type()).time_to_train_s;
        }
    }
}

void player::buy_unit(int unit_level) {
    assert(0 <= unit_level && unit_level < UNITS_PER_AGE);
    std::unique_lock l(m_mutex);
    if (m_units_to_train.size() >= UNITS_QUEUE_SIZE) {
        return;
    }
    auto type = static_cast<unit_type>(static_cast<int>(m_age) * UNITS_PER_AGE + unit_level);
    auto stats = unit::get_stats(type);
    int cost = stats.cost;
    if (m_money < cost) {
        return;
    }
    m_money -= cost;
    m_units_to_train.emplace_back(type);
    if (m_units_to_train.empty()) {
        m_training_time_left = stats.time_to_train_s;
    }
}

void player::buy_cannon(int cannon_level, int slot) {
    assert(0 <= cannon_level && cannon_level < CANNONS_PER_AGE);
    std::unique_lock l(m_mutex);
    assert(0 <= slot && slot < m_cannons.size());
    auto type = static_cast<cannon_type>(static_cast<int>(m_age) * CANNONS_PER_AGE + cannon_level);
    int cost = cannon::get_stats(type).cost;
    if (m_money < cost) {
        return;
    }
    m_money -= cost;
    m_cannons[cannon_level] =
        cannon{type, CANNONS_SLOTS_COORD_X[cannon_level], CANNONS_SLOTS_COORD_Y[cannon_level]};
}

void player::buy_cannon_slot() {
    std::unique_lock l(m_mutex);
    if (m_cannons.size() >= MAX_CANNON_SLOTS) {
        return;
    }
    int cost = CANNONS_SLOTS_COSTS[m_cannons.size()];
    if (m_money < cost) {
        return;
    }
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
    std::unique_lock l(m_mutex);
    if (m_ult_cooldown != 0.0) {
        return;
    }
    m_ult_cooldown = ULT_COOLDOWN;
    const int bullets_amount = 20;
    for (int i = 0; i < bullets_amount; ++i) {
        m_bullets.emplace_back(static_cast<bullet_type>(NUM_OF_CANNONS + static_cast<int>(m_age)),
                               FIELD_LENGTH_PXLS / bullets_amount * i, FIELD_HEIGHT_PXLS,
                               FIELD_LENGTH_PXLS / bullets_amount * i, 0);
    }
}

void player::clear_dead_objects() {
    std::unique_lock l(m_mutex);
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
                                   [](const bullet &bullet_) { return !bullet_.is_alive(); }),
                    m_bullets.end());
    // DO NOT CLEAR TOWER!
    m_units.erase(std::remove_if(m_units.begin() + 1, m_units.end(),
                                 [](const unit &unit_) { return !unit_.is_alive(); }),
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
    assert(!m_units.empty());  // at least tower exists, we don't clear it even if it's dead
    return m_units.front().is_alive();
}
}  // namespace war_of_ages
