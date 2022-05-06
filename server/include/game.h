#ifndef WAR_OF_AGES_GAME_H
#define WAR_OF_AGES_GAME_H

#include <cstdint>
#include <string>
#include "../../common/game_logic/include/game_state.h"
#include "../../common/include/messages_type.h"
#include "../../common/network/include/message.h"
#include "../include/server.h"

namespace war_of_ages {

struct game {
    game(std::string handle_p1, std::string handle_p2) noexcept;

    void apply_command(const std::string &handle, std::unique_ptr<game_command> command) noexcept;
    void update(server &server_ref);
    void user_gave_up(const std::string &handle, server &server_ref);

    [[nodiscard]] bool is_finished() const noexcept;
    [[nodiscard]] const std::string &get_handle_p1() const noexcept;
    [[nodiscard]] const std::string &get_handle_p2() const noexcept;

private:
    [[nodiscard]] static message<messages_type> get_msg_snapshot(const player_snapshot &p_snapshot) noexcept;
    void send_snapshots(server &server_ref) const;

    std::string m_handle_p1;
    std::string m_handle_p2;
    game_state m_state;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_H