#include "player.h"
#include <algorithm>
#include <cassert>
#include <random>
#include <utility>

namespace war_of_ages {

void player::update(player &enemy, float dt) {
    auto &enemies = enemy.m_units;
    for (auto unit_it = m_units.rbegin(); unit_it + 1 != m_units.rend(); ++unit_it) {
        (*unit_it)->update(enemies.back(), (unit_it != m_units.rbegin() ? *(unit_it - 1) : nullptr), dt);
    }
    for (auto &bullet_ : m_bullets) {
        bullet_->update(enemies, dt);
    }
    for (auto &cannon_ : m_cannons) {
        auto bullet_ = cannon_->update(enemies.back(), dt, m_bullet_factory);
        if (bullet_) {
            bullet_->post_create_action(m_handle);
            m_bullets.push_back(bullet_);
        }
    }

    m_ult_cooldown = std::max(m_ult_cooldown - dt, 0.0f);
    m_training_time_left = std::max(m_training_time_left - dt, 0.0f);
    if (m_training_time_left == 0.0 && !m_units_to_train.empty()) {
        m_units.push_front(m_units_to_train[0]);
        std::swap(m_units[0], m_units[1]);  // swap with tower
        m_units[1]->post_create_action();
        m_units_to_train.pop_front();
        if (!m_units_to_train.empty()) {
            m_training_time_left = m_units_to_train[0]->stats().time_to_train_s;
        }
    }
}

bool player::buy_unit(int unit_level) {
    assert(0 <= unit_level && unit_level < UNITS_PER_AGE);
    if (m_units_to_train.size() >= UNITS_QUEUE_SIZE) {
        return false;
    }
    auto type = static_cast<unit_type>(static_cast<int>(m_age) * UNITS_PER_AGE + unit_level);
    auto stats = unit::get_stats(type);
    if (m_money < stats.cost) {
        return false;
    }
    m_money -= stats.cost;
    if (m_units_to_train.empty()) {
        m_training_time_left = stats.time_to_train_s;
    }
    m_units_to_train.push_back(m_unit_factory(type));
    return true;
}

bool player::buy_cannon(int cannon_level, int slot) {
    assert(0 <= cannon_level && cannon_level < CANNONS_PER_AGE);
    assert(0 <= slot && slot < m_cannons.size());
    if (m_cannons[slot]->type() != cannon_type::NONE) {
        return false;
    }
    auto type = static_cast<cannon_type>(static_cast<int>(m_age) * CANNONS_PER_AGE + cannon_level);
    int cost = cannon::get_stats(type).cost;
    if (m_money < cost) {
        return false;
    }
    m_money -= cost;
    m_cannons[slot] = m_cannon_factory(type, {CANNONS_SLOTS_COORD_X[slot], CANNONS_SLOTS_COORD_Y[slot]});
    m_cannons[slot]->post_buy_action();
    return true;
}

bool player::buy_cannon_slot() {
    std::size_t slot = m_cannons.size();
    if (slot >= MAX_CANNON_SLOTS) {
        return false;
    }
    int cost = CANNONS_SLOTS_COSTS[slot];
    if (m_money < cost) {
        return false;
    }
    m_money -= cost;
    m_cannons.push_back(
        m_cannon_factory(cannon_type::NONE, vec2f{CANNONS_SLOTS_COORD_X[slot], CANNONS_SLOTS_COORD_Y[slot]}));
    return true;
}

bool player::sell_cannon(int slot) {
    assert(0 <= slot && slot < CANNONS_PER_AGE);
    if (slot >= m_cannons.size() || m_cannons[slot]->type() == cannon_type::NONE) {
        return false;
    }
    m_money += m_cannons[slot]->stats().cost / 2;
    m_cannons[slot] = m_cannon_factory(cannon_type::NONE, {CANNONS_SLOTS_COORD_X[m_cannons.size()],
                                                           CANNONS_SLOTS_COORD_Y[m_cannons.size()]});
    m_cannons[slot]->post_sell_action();
    return true;
}

bool player::use_ult() {
    if (m_ult_cooldown > 0) {
        return false;
    }
    m_ult_cooldown = ULT_COOLDOWN;
    auto ult_type = static_cast<bullet_type>(NUM_OF_CANNONS + static_cast<int>(m_age));
    vec2f ult_size = bullet::get_stats(ult_type).size;
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution x_offset(0.0, 3.0 * ult_size.x);
    std::uniform_real_distribution y_offset(0.0, 20.0 * ult_size.y);
    const int bullets_amount = 10;
    for (int i = 0; i < bullets_amount; ++i) {
        m_bullets.push_back(
            m_bullet_factory(ult_type,
                             vec2f{FIELD_LENGTH_PXLS / bullets_amount * static_cast<float>(i) +
                                       static_cast<float>(x_offset(gen)),
                                   FIELD_HEIGHT_PXLS + static_cast<float>(y_offset(gen))},
                             vec2f{FIELD_LENGTH_PXLS / bullets_amount * static_cast<float>(i), 0.0f}));
        m_bullets.back()->post_create_action(m_handle);
    }
    return true;
}

bool player::upgrade_age() {
    int age_num = static_cast<int>(m_age);
    if (age_num + 1 == NUM_OF_AGES || m_exp < NEXT_AGE_EXP[age_num]) {
        return false;
    }
    m_age = static_cast<age_type>(age_num + 1);
    m_units[0] = m_unit_factory(static_cast<unit_type>(static_cast<int>(m_units[0]->type()) + 1));
    return true;
}

void player::clear_dead_objects() {
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
                       [](const std::shared_ptr<bullet> &bullet_) { return !bullet_->is_alive(); }),
        m_bullets.end());
    // FIXME: remove '+1' when implemented tower class
    // DO NOT CLEAR TOWER!
    m_units.erase(std::remove_if(m_units.begin() + 1, m_units.end(),
                                 [](const std::shared_ptr<unit> &unit_) { return !unit_->is_alive(); }),
                  m_units.end());
}

// Getters

age_type player::age() const {
    return m_age;
}

int player::exp() const {
    return m_exp;
}

int player::money() const {
    return m_money;
}

std::deque<std::shared_ptr<unit>> player::units() const {
    return m_units;
}

std::vector<std::shared_ptr<bullet>> player::bullets() const {
    return m_bullets;
}

std::vector<std::shared_ptr<cannon>> player::cannons() const {
    return m_cannons;
}

std::deque<std::shared_ptr<unit>> player::units_to_train() const {
    return m_units_to_train;
}

bool player::is_alive() const {
    assert(!m_units.empty());  // at least tower exists, we don't clear it even if it's dead
    return m_units.front()->is_alive();
}

player_snapshot player::snapshot() const {
    return {m_units, m_bullets, m_cannons,      m_units_to_train,    m_age,
            m_exp,   m_money,   m_ult_cooldown, m_training_time_left};
}

void player::set_snapshot(const player_snapshot &snapshot) {
    m_age = snapshot.age;
    m_exp = snapshot.exp;
    m_money = snapshot.money;
    m_ult_cooldown = snapshot.m_ult_cooldown;
    m_units = snapshot.units;
    m_units_to_train = snapshot.units_to_train;
    m_bullets = snapshot.bullets;
    m_training_time_left = snapshot.m_training_time_left;
}

void player::berserk_units(player &enemy) {
    auto &enemies = enemy.m_units;
    for (auto unit_it = m_units.rbegin(); unit_it + 1 != m_units.rend(); ++unit_it) {
        (*unit_it)->berserk(enemies.back());
    }
}

void player::collect_profit(player &enemy) {
    for (const auto &unit_ : enemy.units()) {
        if (!unit_->is_alive()) {
            int cost = unit_->stats().cost;
            m_money += static_cast<int>(1.5f * static_cast<float>(cost));
            m_exp += static_cast<int>((1.0f + (FIELD_LENGTH_PXLS - unit_->position()) / FIELD_LENGTH_PXLS) *
                                      static_cast<float>(cost));
        }
    }
}

player::player(
    std::function<std::shared_ptr<unit>(unit_type)> unit_factory,
    std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)> bullet_factory,
    std::function<std::shared_ptr<cannon>(cannon_type, const vec2f &)> cannon_factory,
    std::string handle)
    : m_unit_factory(std::move(unit_factory)),
      m_bullet_factory(std::move(bullet_factory)),
      m_cannon_factory(std::move(cannon_factory)),
      m_handle(std::move(handle)) {
    m_units.push_back(m_unit_factory(unit_type::STONE_TOWER));
    m_cannons.push_back(
        m_cannon_factory(cannon_type::NONE, {CANNONS_SLOTS_COORD_X[0], CANNONS_SLOTS_COORD_Y[0]}));
}

bool player::add_bullet(bullet_snapshot snapshot) {
    m_bullets.push_back(m_bullet_factory(snapshot.type, {snapshot.x_start, snapshot.y_start},
                                         {snapshot.x_target, snapshot.y_target}));
    m_bullets.back()->post_create_action(m_handle);
    if (m_ult_cooldown <= 0) {
        m_ult_cooldown = ULT_COOLDOWN;
    }
    return true;
}

}  // namespace war_of_ages
