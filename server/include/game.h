#ifndef WAR_OF_AGES_GAME_H
#define WAR_OF_AGES_GAME_H

#include <cstdint>
#include <functional>
#include <string>
#include "../../common/game_logic/include/game_state.h"
#include "../../common/include/messages_type.h"
#include "../../common/network/include/message.h"

namespace war_of_ages {

struct game {
    game(std::string handle_p1,
         std::string handle_p2,
         std::function<void(const std::string &handle_winner, const std::string &handle_loser)>
             game_post_action) noexcept;

    void apply_command(const std::string &handle, std::unique_ptr<game_command> command) noexcept;
    void update();
    void user_gave_up(const std::string &handle);

    [[nodiscard]] bool is_finished() const noexcept;
    [[nodiscard]] const std::string &get_handle_p1() const noexcept;
    [[nodiscard]] const std::string &get_handle_p2() const noexcept;

private:
    [[nodiscard]] static message<messages_type> get_msg_snapshot(const player_snapshot &p_snapshot) noexcept;
    void send_snapshots() const;

    std::string m_handle_p1;
    std::string m_handle_p2;
    game_state m_state;
    std::function<void(const std::string &handle_winner, const std::string &handle_loser)> m_game_post_action;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_H
