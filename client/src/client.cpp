#include "../include/client.h"

namespace war_of_ages {

client_state::client_state(std::string handle_, screen cur_screen_)
    : handle(std::move(handle_)), cur_screen(cur_screen_) {
    player_actions.resize(2);
}

std::string client_state::get_cur_screen_id() const {
    return screen_id.at(cur_screen);
}
std::string client_state::get_handle() const {
    return handle;
}
screen client_state::get_cur_screen() const {
    return cur_screen;
}
std::shared_ptr<tournament> client_state::get_cur_tournament() const {
    return cur_tournament;
}

void client_state::set_cur_screen(screen s) {
    cur_screen = s;
}
void client_state::set_cur_game_state(std::shared_ptr<game_state> st) {
    cur_game_state = std::move(st);
}

std::shared_ptr<game_state> client_state::get_cur_game_state() const {
    return cur_game_state;
}
vec2f client_state::get_view_center() const noexcept {
    return view_center;
}
void client_state::set_view_center(const vec2f &v) {
    view_center = v;
}
void client_state::add_action(int player, std::unique_ptr<game_command> cmd) {
    player_actions[player].emplace_back(cmd.release());
}
const std::vector<std::vector<std::unique_ptr<game_command>>> &client_state::get_player_actions() const {
    return player_actions;
}
void client_state::clear_actions() {
    player_actions[0].clear();
    player_actions[1].clear();
}

}  // namespace war_of_ages
