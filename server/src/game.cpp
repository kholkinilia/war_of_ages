#include "../include/game.h"
#include <ctime>

namespace war_of_ages {
game::game(std::string handle_p1, std::string handle_p2) noexcept
    : m_handle_p1(std::move(handle_p1)),
      m_handle_p2(std::move(handle_p2)),
      m_state(1.f * clock() / CLOCKS_PER_SEC) {
}

void game::apply_command(std::unique_ptr<game_command> command) {
}

void game::update() {
}

message<messages_type> game::get_snapshot(const std::string &handle) noexcept {
}

void game::send_snapshots() {
}
}  // namespace war_of_ages