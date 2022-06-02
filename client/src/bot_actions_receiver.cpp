#include "../include/bot_actions_receiver.h"
#include <fstream>
#include <iostream>
#include <random>
#include "../include/client.h"
#include "../include/sprite_supplier.h"

namespace war_of_ages {

std::mt19937 gen(time(nullptr));

bool bot_actions_receiver::read = false;

bot_actions_receiver::bot_actions_receiver() {
    positions.resize(2);
    //    if (!read)
    //        read_from_file(1);
    std::unique_lock l(sprite_supplier::m);
    while (!read) {
        sprite_supplier::cond_var.wait(l);
    }
    assert(read);
}

state bot_actions_receiver::get_state(const std::pair<player_snapshot, player_snapshot> &p, int player) {
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
    answer.damage.first = std::min(answer.damage.first / 5, static_cast<int>(Q_table[0].size()) - 1);
    answer.damage.second = std::min(answer.damage.second / 5, static_cast<int>(Q_table[0].size()) - 1);

    if (player == 1) {
        std::swap(answer.damage.first, answer.damage.second);
    }

    return answer;
}

static float get_max(std::vector<float> v) {
    return *std::max_element(v.begin(), v.end());
}

static float get_reward(state last_state, state new_state) {
    return static_cast<float>(((new_state.damage.first - new_state.damage.second) -
                               (last_state.damage.first - last_state.damage.second))) /
           static_cast<float>(new_state.damage.first + 1);
    // return (rand() % 2 ? 1 : -1) * 2 * static_cast<float>(gen()) / INT32_MAX;
}

bot_actions_receiver::action bot_actions_receiver::get_action(state state, int player) {
    std::vector<float> v(static_cast<int>(action::NONE) + 1);
    std::iota(v.begin(), v.end(), 0);
    auto delta = *std::min_element(Q_table[player][state.damage.first][state.damage.second].begin(),
                                   Q_table[player][state.damage.first][state.damage.second].end());
    delta += 1e-6;
    std::for_each(Q_table[player][state.damage.first][state.damage.second].begin(),
                  Q_table[player][state.damage.first][state.damage.second].end(),
                  [&delta](float &d) { d += delta; });
    std::piecewise_constant_distribution<float> distribution(
        v.begin(), v.end(), Q_table[player][state.damage.first][state.damage.second].begin());
    std::for_each(Q_table[player][state.damage.first][state.damage.second].begin(),
                  Q_table[player][state.damage.first][state.damage.second].end(),
                  [&delta](float &d) { d -= delta; });
    auto ans = static_cast<action>(std::round(distribution(gen)));
    return ans;
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
    if (last_state.damage.first != -1) {
        float reward = get_reward(last_state, new_state);
        Q_table[player][last_state.damage.first][last_state.damage.first][static_cast<int>(last_action)] +=
            learning_rate *
            (reward + gamma * get_max(Q_table[player][new_state.damage.first][new_state.damage.first]) -
             Q_table[player][last_state.damage.first][last_state.damage.first]
                    [static_cast<int>(last_action)]);
    }
    positions[player].emplace(last_state.damage);
    action cur_action = get_action(new_state, player);

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

void bot_actions_receiver::write_to_file(int player) {
    std::ofstream out("bot_config" + std::to_string(player) + ".txt");
    std::cout << player + 1 << ") Positions reached: " << positions[player].size() << std::endl;
    int counter = 0;
    for (const auto &a : Q_table[player]) {
        for (const auto &b : a) {
            for (float e : b) {
                out << e << " ";
                if (abs(e) > 1e-7) {
                    counter++;
                }
            }
            out << "\n";
        }
    }
    std::cout << "Non-zero values in table: " << counter << std::endl;
    out.close();
}

void bot_actions_receiver::read_from_file(int player) {
    if (Q_table.empty())
        Q_table.resize(2, std::vector<std::vector<std::vector<float>>>(
                              300, std::vector<std::vector<float>>(
                                       300, std::vector<float>(static_cast<int>(action::NONE) + 1, 0))));
    std::string str;
    std::stringstream ss;
    std::ifstream in("bot_config" + std::to_string(player) + ".txt");
    int counter = 0;
    for (auto &a : Q_table[player]) {
        for (auto &b : a) {
            std::getline(in, str);
            ss = std::stringstream(str);
            for (float &e : b) {
                ss >> e;
                if (abs(e) > 1e-7) {
                    counter++;
                }
            }
        }
    }
    if (player)
        read = true;
    std::cout << counter << std::endl;
    in.close();
}

}  // namespace war_of_ages