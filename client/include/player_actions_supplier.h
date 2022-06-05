#ifndef WAR_OF_AGES_PLAYER_ACTIONS_SUPPLIER_H
#define WAR_OF_AGES_PLAYER_ACTIONS_SUPPLIER_H

#include <memory>
#include <queue>
#include <vector>
#include "game_commands.h"

namespace war_of_ages::client {

struct player_actions_supplier {
private:
    std::deque<std::unique_ptr<game_command>> m_actions;
    std::mutex m_mutex;

    player_actions_supplier() = default;

public:
    player_actions_supplier(const player_actions_supplier &) = delete;
    player_actions_supplier(player_actions_supplier &&) = delete;
    player_actions_supplier &operator=(const player_actions_supplier &) = delete;
    player_actions_supplier &operator=(player_actions_supplier &&) = delete;

    ~player_actions_supplier() = default;

    [[nodiscard]] std::vector<std::unique_ptr<game_command>> get_actions();
    void add_action(std::unique_ptr<game_command> cmd);
    void clear_actions();

    static player_actions_supplier &instance();
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_PLAYER_ACTIONS_SUPPLIER_H
