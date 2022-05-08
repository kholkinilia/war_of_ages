#ifndef WAR_OF_AGES_GAME_HANDLER_H
#define WAR_OF_AGES_GAME_HANDLER_H

#include <memory>
#include "../../common/game_logic/include/game_state.h"
#include "actions_supplier.h"

namespace war_of_ages {

struct game_handler {
public:
    enum class player_type { PLAYER, BOT };
    explicit game_handler(std::vector<player_type> types_);
    [[nodiscard]] std::shared_ptr<game_state> get_cur_game_state() const;
    void reset();
    std::vector<std::unique_ptr<game_command>> get_actions(int index);
    void append_action(int index, std::unique_ptr<game_command> cmd);
    void clear_actions();
    void set_receiver(int index, player_type type);
    player_type get_type(int index);

private:
    std::shared_ptr<game_state> cur_game_state;
    std::vector<std::shared_ptr<actions_supplier>> suppliers;
    std::vector<player_type> types;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_HANDLER_H
