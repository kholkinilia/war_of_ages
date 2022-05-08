#ifndef WAR_OF_AGES_SINGLE_PLAYER_HANDLER_H
#define WAR_OF_AGES_SINGLE_PLAYER_HANDLER_H

#include <memory>
#include "../../common/game_logic/include/game_state.h"

namespace war_of_ages {

struct single_player_handler {
    enum class player_type { PLAYER, BOT };

private:
    std::unique_ptr<game_state> m_game_state;
    player_type m_player_type = player_type::PLAYER;

    single_player_handler() = default;

public:
    single_player_handler(const single_player_handler &) = delete;
    single_player_handler(single_player_handler &&) = delete;
    single_player_handler &operator=(const single_player_handler &) = delete;
    single_player_handler &operator=(single_player_handler &&) = delete;

    void update_game();
    void start_game();
    void finish_game();

    void change_player_type(player_type new_type);
    [[nodiscard]]player_type get_type() const noexcept;
    [[nodiscard]] std::pair<player_snapshot, player_snapshot> get_snapshot() const noexcept;

    [[nodiscard]]static single_player_handler &instance();

};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SINGLE_PLAYER_HANDLER_H
