#ifndef WAR_OF_AGES_ACTIONS_RECIEVER_H
#define WAR_OF_AGES_ACTIONS_RECIEVER_H

#include <memory>
#include <vector>
#include "../../game_logic/include/game_commands.h"

namespace war_of_ages {

struct actions_receiver {
public:
    [[nodiscard]] virtual std::vector<std::unique_ptr<game_command>> const &get_actions(
        player_snapshot p) = 0;
    void append(std::unique_ptr<game_command> cmd) {
        actions.push_back(std::move(cmd));
    }
    void clear() {
        actions.clear();
    }

protected:
    std::vector<std::unique_ptr<game_command>> actions;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_ACTIONS_RECIEVER_H
