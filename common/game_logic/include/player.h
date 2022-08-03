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

struct player_snapshot {  // it is not used for snapshot purposes actually (so shared_ptr is ok)
    std::deque<std::shared_ptr<unit>> units;
    std::vector<std::shared_ptr<bullet>> bullets;
    std::vector<std::shared_ptr<cannon>> cannons;
    std::deque<std::shared_ptr<unit>> units_to_train;

    age_type age;
    int exp;
    int money;
    float m_ult_cooldown;
    float m_training_time_left;
};

struct player {
    explicit player(
        std::function<std::shared_ptr<unit>(unit_type)> unit_factory,
        std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)> bullet_factory,
        std::function<std::shared_ptr<cannon>(cannon_type, const vec2f &)> cannon_factory,
        std::string handle = "");

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
    bool add_bullet(bullet_snapshot snapshot);

    void set_snapshot(const player_snapshot &snapshot);

    // Getters

    [[nodiscard]] enum age_type age() const;
    [[nodiscard]] int exp() const;
    [[nodiscard]] int money() const;
    [[nodiscard]] std::deque<std::shared_ptr<unit>> units() const;
    [[nodiscard]] std::vector<std::shared_ptr<bullet>> bullets() const;
    [[nodiscard]] std::vector<std::shared_ptr<cannon>> cannons() const;
    [[nodiscard]] std::deque<std::shared_ptr<unit>> units_to_train() const;
    [[nodiscard]] bool is_alive() const;
    [[nodiscard]] player_snapshot snapshot() const;

private:
    age_type m_age = age_type::STONE;
    int m_exp = 10000;
    int m_money = INITIAL_MONEY;
    float m_ult_cooldown = 0.0f;
    std::deque<std::shared_ptr<unit>> m_units;
    std::vector<std::shared_ptr<bullet>> m_bullets = {};
    std::vector<std::shared_ptr<cannon>> m_cannons = {};
    float m_training_time_left = 0.0;
    std::deque<std::shared_ptr<unit>> m_units_to_train = {};
    std::string m_handle;

    std::function<std::shared_ptr<unit>(unit_type)> m_unit_factory;
    std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)> m_bullet_factory;
    std::function<std::shared_ptr<cannon>(cannon_type, const vec2f &)> m_cannon_factory;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_PLAYER_H
