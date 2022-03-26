#ifndef WAR_OF_AGES_GAME_HANDLER_H
#define WAR_OF_AGES_GAME_HANDLER_H

#include <memory>
#include "../../game_logic/include/game_state.h"
#include "actions_receiver.h"

namespace war_of_ages {

struct game_handler {
private:
    std::shared_ptr<game_state> cur_game_state;
    std::vector<std::shared_ptr<actions_receiver>> receivers;
public:
    explicit game_handler(std::vector<std::shared_ptr<actions_receiver>> receivers_);
    [[nodiscard]] std::shared_ptr<game_state> get_cur_game_state() const;
    void reset();
    std::vector<std::unique_ptr<game_command>> const &get_actions(int index, player_snapshot p);
    void append_action(int index, std::unique_ptr<game_command> cmd);
    void clear_actions();
};

}

#endif  // WAR_OF_AGES_GAME_HANDLER_H
