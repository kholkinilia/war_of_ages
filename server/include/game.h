#ifndef WAR_OF_AGES_GAME_H
#define WAR_OF_AGES_GAME_H

#include <cstdint>
#include <string>
#include "../../common/game_logic/include/game_state.h"
#include "../../common/include/messages_type.h"
#include "../../common/network/include/message.h"

namespace war_of_ages {

struct game {
    game(std::string handle_p1, std::string handle_p2) noexcept;

    void apply_command(std::unique_ptr<game_command> command);
    void update();

private:
    [[nodiscard]] message<messages_type> get_snapshot(const std::string &handle) noexcept;
    void send_snapshots();

    std::string m_handle_p1;
    std::string m_handle_p2;
    game_state m_state;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_H
