#ifndef WAR_OF_AGES_PLAYER_H
#define WAR_OF_AGES_PLAYER_H

#include "bullet.h"
#include "unit.h"
//#include "cannon.h"
#include <queue>
#include <vector>

namespace war_of_ages {
struct player {
    player() noexcept;

    void update(std::vector<unit> &enemies, int time);  // need all enemies for ult
    void buy_unit(int unit_level);     // unit_level in [0, NUM_OF_UNITS=3), unit_type = NUM_OF_AGES *
                                       // m_age + unit_level ???
    void buy_cannon(int cannon_type);  // the same remark
    void buy_cannon_slot();
    void sell_cannon(int slot);
    void use_ult();

    // Getters

    [[nodiscard]] int age() const;
    [[nodiscard]] int exp() const;
    [[nodiscard]] int money() const;
    [[nodiscard]] std::queue<unit> units() const;       // Copy, not by & cause of multithreading
    [[nodiscard]] std::vector<bullet> bullets() const;  // The same remark
    //[[nodiscard]] std::vector<unit> cannons() const;  // The same remark
    [[nodiscard]] std::queue<int> units_to_train() const;
    [[nodiscard]] bool is_alive() const;

private:
    int m_age = 0;  // TODO: replace with enum
    int m_exp = 0;
    int m_money;  // TODO: = INITIAL_MONEY = ?
    int m_last_ult = 0;
    std::queue<unit> m_units = {};
    std::vector<bullet> m_bullets = {};
    //  std::vector<cannon> m_cannons = {};

    std::queue<int> m_units_to_train;  // TODO: should think about limit of queue's size (5?)
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_PLAYER_H
