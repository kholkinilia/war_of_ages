#include "bot_actions_supplier.h"
#include <filesystem>
#include <iostream>
#include <random>
#include "client.h"
#include "sprite_supplier.h"

namespace war_of_ages::client {

std::mt19937 gen(time(nullptr));

bool bot_actions_supplier::read = false;

bot_actions_supplier::bot_actions_supplier() {
    std::unique_lock l(sprite_supplier::get_mutex());
    while (!read) {
        sprite_supplier::sprite_supplier::get_cond_var().wait(l);
    }
    assert(read);
}

state bot_actions_supplier::get_state(const std::pair<player_snapshot, player_snapshot> &p) {
    state answer;
    answer.damage = {0, 0};
    for (auto unit : p.first.units) {
        answer.damage.first += unit->stats().damage;
    }
    for (auto unit : p.second.units) {
        answer.damage.second += unit->stats().damage;
    }
    for (auto unit : p.first.units_to_train) {
        answer.damage.first += unit->stats().damage;
    }
    for (auto unit : p.second.units_to_train) {
        answer.damage.second += unit->stats().damage;
    }
    for (auto cannon : p.first.cannons) {
        answer.damage.first += bullet::get_stats(cannon->stats().b_type).damage;
    }
    for (auto cannon : p.second.cannons) {
        answer.damage.second += bullet::get_stats(cannon->stats().b_type).damage;
    }
    answer.damage.first = std::min(answer.damage.first / 5, static_cast<int>(Q_table[0].size()) - 1);
    answer.damage.second = std::min(answer.damage.second / 5, static_cast<int>(Q_table[0].size()) - 1);

    return answer;
}

static float get_max(std::vector<float> v) {
    return *std::max_element(v.begin(), v.end());
}

static float get_reward(state last_state, state new_state) {
    return static_cast<float>(((new_state.damage.first - new_state.damage.second) -
                               (last_state.damage.first - last_state.damage.second))) /
           static_cast<float>(new_state.damage.first + 1);
}

bot_actions_supplier::action bot_actions_supplier::get_action(state state) {
    std::vector<float> v(static_cast<int>(action::NONE) + 1);
    std::iota(v.begin(), v.end(), 0);
    auto delta = *std::min_element(Q_table[state.damage.first][state.damage.second].begin(),
                                   Q_table[state.damage.first][state.damage.second].end());
    delta += 1e-6;
    std::for_each(Q_table[state.damage.first][state.damage.second].begin(),
                  Q_table[state.damage.first][state.damage.second].end(), [&delta](float &d) { d += delta; });
    std::piecewise_constant_distribution<float> distribution(
        v.begin(), v.end(), Q_table[state.damage.first][state.damage.second].begin());
    std::for_each(Q_table[state.damage.first][state.damage.second].begin(),
                  Q_table[state.damage.first][state.damage.second].end(), [&delta](float &d) { d -= delta; });
    auto ans = static_cast<action>(std::round(distribution(gen)));
    return ans;
}

std::vector<std::unique_ptr<game_command>> bot_actions_supplier::get_actions(
    const std::pair<player_snapshot, player_snapshot> &p) {
    //    std::vector<std::unique_ptr<game_command>> res;          // uncomment if you need a stupid bot
    //    res.push_back(std::make_unique<buy_unit_command>(0));
    //
    //    return res;
    auto new_state = get_state(p);
    if (last_state.damage.first != -1) {
        float reward = get_reward(last_state, new_state);
        Q_table[last_state.damage.first][last_state.damage.first][static_cast<int>(last_action)] +=
            learning_rate *
            (reward + gamma * get_max(Q_table[new_state.damage.first][new_state.damage.first]) -
             Q_table[last_state.damage.first][last_state.damage.first][static_cast<int>(last_action)]);
    }
    action cur_action = get_action(new_state);

    int slot = -1;
    auto cannons = p.first.cannons;
    for (int j = 0; j < cannons.size(); j++) {
        auto c = cannons[j];
        if (c->type() == cannon_type::NONE) {
            slot = j;
            break;
        }
    }

    std::vector<std::unique_ptr<game_command>> actions;
    switch (cur_action) {
        case action::BUY_UNIT_1:
            actions.emplace_back(std::make_unique<buy_unit_command>(0));
            break;
        case action::BUY_UNIT_2:
            actions.emplace_back(std::make_unique<buy_unit_command>(1));
            break;
        case action::BUY_UNIT_3:
            actions.emplace_back(std::make_unique<buy_unit_command>(2));
            break;
        case action::BUY_CANNON_1:
            if (slot >= 0)
                actions.emplace_back(std::make_unique<buy_cannon_command>(0, slot));
            break;
        case action::BUY_CANNON_2:
            if (slot >= 0)
                actions.emplace_back(std::make_unique<buy_cannon_command>(1, slot));
            break;
        case action::BUY_CANNON_3:
            if (slot >= 0)
                actions.emplace_back(std::make_unique<buy_cannon_command>(2, slot));
            break;
        case action::SELL_CANNON_1:
            actions.emplace_back(std::make_unique<sell_cannon_command>(0));
            break;
        case action::SELL_CANNON_2:
            actions.emplace_back(std::make_unique<sell_cannon_command>(1));
            break;
        case action::SELL_CANNON_3:
            actions.emplace_back(std::make_unique<sell_cannon_command>(2));
            break;
        case action::BUY_SLOT:
            actions.emplace_back(std::make_unique<buy_cannon_slot_command>());
            break;
        case action::NEW_ERA:
            actions.emplace_back(std::make_unique<upgrade_age_command>());
            break;
        case action::USE_ULT:
            actions.emplace_back(std::make_unique<use_ult_command>());
            break;
        default:
            break;
    }

    last_state = new_state;
    last_action = cur_action;

    return actions;
}

void bot_actions_supplier::read_from_file() {
    Q_table.resize(
        300, std::vector<std::vector<float>>(300, std::vector<float>(static_cast<int>(action::NONE) + 1, 0)));
    std::string str;
    std::stringstream ss;
    std::ifstream in("../../client/configs/bot_config.txt");
    for (auto &a : Q_table) {
        for (auto &b : a) {
            std::getline(in, str);
            ss = std::stringstream(str);
            for (float &e : b) {
                ss >> e;
            }
        }
    }
    read = true;
    in.close();
}

bot_actions_supplier &bot_actions_supplier::instance() {
    static bot_actions_supplier supplier;
    return supplier;
}

void bot_actions_supplier::write_to_file() {
    std::ofstream out("bot_config.txt");
    for (const auto &a : Q_table) {
        for (const auto &b : a) {
            for (float e : b) {
                out << e << " ";
            }
            out << "\n";
        }
    }
    out.close();
}
}  // namespace war_of_ages::client