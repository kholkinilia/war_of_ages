#ifndef WAR_OF_AGES_PLAYER_H
#define WAR_OF_AGES_PLAYER_H

#include <deque>
#include <functional>
#include <mutex>
#include <vector>
#include "age.h"
#include "bullet.h"
#include "cannon.h"
#include "game_constants.h"
#include "unit.h"

namespace war_of_ages {

struct player_snapshot {
    std::deque<std::shared_ptr<unit>> units;
    std::vector<bullet> bullets;
    std::vector<cannon> cannons;
    std::deque<std::shared_ptr<unit>> units_to_train;

    age_type age;
    int exp;
    int money;
    float m_ult_cooldown;
    float m_training_time_left;
};

struct player {
    explicit player(std::function<std::shared_ptr<unit>(unit_type)> unit_factory);

    void update(player &enemy, float dt);
    void berserk_units(player &enemy);
    void collect_profit(player &enemy);
    void clear_dead_objects();

    bool buy_unit(int unit_level);
    bool buy_cannon(int cannon_level, int slot);
    bool buy_cannon_slot();
    bool sell_cannon(int slot);
    bool use_ult();
    bool upgrade_age();

    void set_snapshot(const player_snapshot &snapshot);

    // Getters

    [[nodiscard]] enum age_type age() const;
    [[nodiscard]] int exp() const;
    [[nodiscard]] int money() const;
    [[nodiscard]] std::deque<std::shared_ptr<unit>> units() const;
    [[nodiscard]] std::vector<bullet> bullets() const;
    [[nodiscard]] std::vector<cannon> cannons() const;
    [[nodiscard]] std::deque<std::shared_ptr<unit>> units_to_train() const;
    [[nodiscard]] bool is_alive() const;
    [[nodiscard]] player_snapshot snapshot() const;

private:
    age_type m_age = age_type::STONE;
    int m_exp = 10000;
    int m_money = INITIAL_MONEY;
    float m_ult_cooldown = 0.0f;
    std::deque<std::shared_ptr<unit>> m_units;
    std::vector<bullet> m_bullets = {};
    std::vector<cannon> m_cannons = {
        cannon{cannon_type::NONE, {CANNONS_SLOTS_COORD_X[0], CANNONS_SLOTS_COORD_Y[0]}}};
    float m_training_time_left = 0.0;
    std::deque<std::shared_ptr<unit>> m_units_to_train = {};

    std::function<std::shared_ptr<unit>(unit_type)> m_unit_factory;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_PLAYER_H
