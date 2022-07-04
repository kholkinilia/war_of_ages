#include "player_actions_supplier.h"
#include <mutex>

namespace war_of_ages::client {

std::vector<std::unique_ptr<game_command>> player_actions_supplier::get_actions() {
    std::unique_lock lock(m_mutex);
    std::vector<std::unique_ptr<game_command>> res;
    std::size_t number_to_pop = m_actions.size();
    for (std::size_t i = 0; i < number_to_pop; i++) {
        res.emplace_back(std::move(m_actions.front()));
        m_actions.pop_front();
    }
    return res;
}

void player_actions_supplier::add_action(std::unique_ptr<game_command> cmd) {
    std::unique_lock lock(m_mutex);
    m_actions.push_back(std::move(cmd));
}

void player_actions_supplier::clear_actions() {
    std::unique_lock lock(m_mutex);
    m_actions.clear();
}

player_actions_supplier &player_actions_supplier::instance() {
    static player_actions_supplier supplier;
    return supplier;
}

}  // namespace war_of_ages::client