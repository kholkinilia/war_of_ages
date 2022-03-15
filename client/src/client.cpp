#include "../include/client.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

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

std::shared_ptr<game_state> client_state::get_cur_game_state() const {
    return cur_game_state;
}

}  // namespace war_of_ages
