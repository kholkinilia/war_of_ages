#include "../include/client.h"
#include "../include/bot_actions_receiver.h"

namespace war_of_ages {

client_state::client_state(std::string handle_, screen_handler::screen_type cur_screen_)
    : handle(std::move(handle_)), cur_screen(cur_screen_), cur_game(nullptr) {
}

std::string client_state::get_cur_screen_id() const {
    return screen_handler::screen_id.at(cur_screen);
}
std::string client_state::get_handle() const {
    return handle;
}
screen_handler::screen_type client_state::get_cur_screen() const {
    return cur_screen;
}
std::shared_ptr<tournament> client_state::get_cur_tournament() const {
    return cur_tournament;
}

void client_state::set_cur_screen(screen_handler::screen_type s) {
    cur_screen = s;
}

std::shared_ptr<game_state> client_state::get_cur_game_state() const {
    return cur_game ? cur_game->get_cur_game_state() : nullptr;
}
vec2f client_state::get_view_center() const noexcept {
    return view_center;
}
void client_state::set_view_center(const vec2f &v) {
    view_center = v;
}
void client_state::reset_game() {
    cur_game->reset();
}
void client_state::create_game(game_mode mode) {
    std::vector<game_handler::player_type> types{game_handler::player_type::PLAYER,
                                                 game_handler::player_type::BOT};
    if (mode == game_mode::MULTI) {
        types[1] = game_handler::player_type::PLAYER;
    }
    cur_game = std::make_shared<game_handler>(types);
}
std::shared_ptr<game_handler> client_state::get_cur_game() const noexcept {
    return cur_game;
}

}  // namespace war_of_ages
