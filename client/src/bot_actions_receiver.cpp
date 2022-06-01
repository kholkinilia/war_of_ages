#include "../include/bot_actions_receiver.h"
#include <fstream>
#include <iostream>
#include <random>
#include "../include/client.h"
#include "../include/sprite_supplier.h"

namespace war_of_ages {

bool bot_actions_receiver::read = false;

std::mt19937 gen(time(nullptr));

bot_actions_receiver::bot_actions_receiver() {
    std::unique_lock l(sprite_supplier::m);
    while (!read) {
        sprite_supplier::cond_var.wait(l);
    }
    assert(read);
}

static state get_state(const std::pair<player_snapshot, player_snapshot> &p, int player) {
    state answer;
    answer.damage = {0, 0};
    for (auto unit : p.first.units) {
        answer.damage.first += unit.stats().damage;
    }
    for (auto unit : p.second.units) {
        answer.damage.second += unit.stats().damage;
    }
    for (auto unit : p.first.units_to_train) {
        answer.damage.first += unit.stats().damage;
    }
    for (auto unit : p.second.units_to_train) {
        answer.damage.second += unit.stats().damage;
    }
    for (auto cannon : p.first.cannons) {
        answer.damage.first += bullet::get_stats(cannon.stats().b_type).damage;
    }
    for (auto cannon : p.second.cannons) {
        answer.damage.second += bullet::get_stats(cannon.stats().b_type).damage;
    }
    answer.damage.first = std::min(answer.damage.first / 25, 99);
    answer.damage.second = std::min(answer.damage.second / 25, 99);

    answer.hp = {std::max(p.first.units[0].remaining_hp() / 500, 0),
                 std::max(p.second.units[0].remaining_hp() / 500, 0)};
    if (player == 1) {
        std::swap(answer.damage.first, answer.damage.second);
        std::swap(answer.hp.first, answer.hp.second);
    }

    return answer;
}

static double get_max(const std::vector<double> &v) {
    return *std::max_element(v.begin(), v.end());
}

static double get_reward(state last_state, state new_state) {
    return static_cast<double>(((new_state.damage.first - new_state.damage.second) -
                                (last_state.damage.first - last_state.damage.second))) +
           (new_state.hp.first - new_state.hp.second) - (last_state.hp.first - last_state.hp.second);
}

bot_actions_receiver::action bot_actions_receiver::get_action(state state) {
    std::vector<double> v(static_cast<int>(action::NONE) + 1);
    std::iota(v.begin(), v.end(), 0);
    auto delta = *std::min_element(
        Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].begin(),
        Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].end());
    delta += 1e-6;
    std::for_each(Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].begin(),
                  Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].end(),
                  [&delta](double &d) { d += delta; });
    std::piecewise_constant_distribution<double> distribution(
        v.begin(), v.end(),
        Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].begin());
    std::for_each(

        Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].begin(),
        Q_table[state.damage.first][state.damage.second][state.hp.first][state.hp.second].end(),
        [&delta](double &d) { d -= delta; });
    return static_cast<action>(std::round(distribution(gen)));
}

std::vector<std::unique_ptr<game_command>> const &bot_actions_receiver::get_actions(
    std::pair<player_snapshot, player_snapshot> p,
    int player) {
    if (current_state.get_counter() != 1) {
        assert(!positions.empty());
    } else {
        if (last_state.damage.first != -1) {
            assert(!positions.empty());
        }
    }

    auto new_state = get_state(p, player);
    positions.emplace(new_state.damage, std::make_pair(0, 0));
    if (last_state.damage.first != -1) {
        double reward = get_reward(last_state, new_state);
        Q_table[last_state.damage.first][last_state.damage.first][last_state.hp.first][last_state.hp.first]
               [static_cast<int>(last_action)] +=
            learning_rate * (reward +
                             gamma * get_max(Q_table[new_state.damage.first][new_state.damage.first]
                                                    [new_state.hp.first][new_state.hp.first]) -
                             Q_table[last_state.damage.first][last_state.damage.first][last_state.hp.first]
                                    [last_state.hp.first][static_cast<int>(last_action)]);
    }
    action cur_action = get_action(new_state);

    int slot = -1;
    auto cannons = player == 0 ? p.first.cannons : p.second.cannons;
    for (int j = 0; j < cannons.size(); j++) {
        auto c = cannons[j];
        if (c.type() == cannon_type::NONE) {
            slot = j;
            break;
        }
    }

    actions.clear();
    switch (cur_action) {
        case action::BUY_UNIT_1:
            append(std::make_unique<buy_unit_command>(0));
            break;
        case action::BUY_UNIT_2:
            append(std::make_unique<buy_unit_command>(1));
            break;
        case action::BUY_UNIT_3:
            append(std::make_unique<buy_unit_command>(2));
            break;
        case action::BUY_CANNON_1:
            if (slot >= 0)
                append(std::make_unique<buy_cannon_command>(0, slot));
            break;
        case action::BUY_CANNON_2:
            if (slot >= 0)
                append(std::make_unique<buy_cannon_command>(1, slot));
            break;
        case action::BUY_CANNON_3:
            if (slot >= 0)
                append(std::make_unique<buy_cannon_command>(2, slot));
            break;
        case action::SELL_CANNON_1:
            append(std::make_unique<sell_cannon_command>(0));
            break;
        case action::SELL_CANNON_2:
            append(std::make_unique<sell_cannon_command>(1));
            break;
        case action::SELL_CANNON_3:
            append(std::make_unique<sell_cannon_command>(2));
            break;
        case action::BUY_SLOT:
            append(std::make_unique<buy_cannon_slot_command>());
            break;
        /*case action::NEW_ERA:
            append(std::make_unique<upgrade_age_command>());
            break;
        */
        case action::USE_ULT:
            append(std::make_unique<use_ult_command>());
            break;
        default:
            break;
    }

    last_state = new_state;
    last_action = cur_action;

    return actions;
}
std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> bot_actions_receiver::get_positions() {
    return positions;
}
void bot_actions_receiver::write_to_file() {
    std::ofstream out("bot_config.txt");
    std::cout << "Positions reached: " << positions.size() << std::endl;
    int i1 = 0, i2, i3, i4, i5, counter = 0;
    for (const auto &a : Q_table) {
        i2 = 0;
        for (const auto &b : a) {
            i3 = 0;
            for (const auto &c : b) {
                i4 = 0;
                for (const auto &d : c) {
                    i5 = 0;
                    for (double e : d) {
                        out << e << " ";
                        if (abs(e) > 1e-7) {
                            counter++;
                        }
                        i5++;
                    }
                    out << "\n";
                    i4++;
                }
                i3++;
            }
            i2++;
        }
        i1++;
    }
    std::cout << "Non-zero values in table: " << counter << std::endl;
    out.close();
}

void bot_actions_receiver::read_from_file() {
    std::cout << "Start reading\n";
    std::ifstream in("bot_config.txt");
    Q_table.resize(100,
                   std::vector<std::vector<std::vector<std::vector<double>>>>(
                       100, std::vector<std::vector<std::vector<double>>>(
                                11, std::vector<std::vector<double>>(
                                        11, std::vector<double>(static_cast<int>(action::NONE) + 1, 0)))));
    for (const auto &a : Q_table) {
        for (const auto &b : a) {
            for (const auto &c : b) {
                for (const auto &d : c) {
                    for (double e : d) {
                        in >> e;
                    }
                }
            }
        }
        std::cout << "Still...\n";
    }
    read = true;
    std::cout << "Done\n";
    in.close();
}

}  // namespace war_of_ages