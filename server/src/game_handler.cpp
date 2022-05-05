#include "../include/game_handler.h"

namespace war_of_ages {
game_handler &game_handler::instance() noexcept {
    static game_handler inst;
    return inst;
}

void game_handler::add_game(const std::string &first_handle, const std::string &second_handle) {
}

void game_handler::apply_command(const std::string &handle, std::unique_ptr<game_command> command) {
}

void game_handler::update_games() {
}
}  // namespace war_of_ages