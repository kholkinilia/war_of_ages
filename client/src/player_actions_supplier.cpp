#include "../include/player_actions_supplier.h"
#include <mutex>

namespace war_of_ages {

std::vector<std::unique_ptr<game_command>> player_actions_supplier::pop_actions() {
    std::unique_lock lock(m_mutex);
    std::vector<std::unique_ptr<game_command>> res;
    std::size_t number_to_pop = actions.size();
    for (std::size_t i = 0; i < number_to_pop; i++) {
        res.emplace_back(std::move(actions.front()));
        actions.pop_front();
    }
    return res;
}

void player_actions_supplier::add_action(std::unique_ptr<game_command> cmd) {
    std::unique_lock lock(m_mutex);
    actions.push_back(std::move(cmd));
}

void player_actions_supplier::clear_actions() {
    std::unique_lock lock(m_mutex);
    actions.clear();
}

}  // namespace war_of_ages