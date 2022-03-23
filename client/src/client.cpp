#include "../include/client.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <utility>

namespace war_of_ages {

client_state::client_state(tgui::String handle_, screen cur_screen_)
    : handle(std::move(handle_)), cur_screen(cur_screen_) {
}

tgui::String client_state::get_cur_screen_id() const {
    return screen_id.at(cur_screen);
}
tgui::String client_state::get_handle() const {
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
vec2f client_state::get_view_center() {
    return view_center;
}
void client_state::set_view_center(const vec2f &v) {
    view_center = v;
}
std::shared_ptr<sound_player> client_state::get_audio_player() const {
    return audio_player;
}
void client_state::create_audio_player() {
    audio_player = std::make_shared<sound_player>();
}

}  // namespace war_of_ages
