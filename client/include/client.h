#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <SFML/Audio.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui, tgui::String
#include <memory>
#include "../../game_logic/include/game_state.h"
#include "screen_defines.h"
#include "sound_player.h"
#include "tournament.h"

namespace war_of_ages {

// Will be implemented to the end by Timur

struct client_state {
private:
    tgui::String handle;
    screen cur_screen;
    std::shared_ptr<tournament> cur_tournament;
    std::shared_ptr<game_state> cur_game_state;
    std::shared_ptr<sound_player> audio_player;
    vec2f view_center;

public:
    explicit client_state(tgui::String handle_ = "handle", screen cur_screen_ = screen::START_SCREEN);

    [[nodiscard]] tgui::String get_handle() const;
    [[nodiscard]] tgui::String get_cur_screen_id() const;
    [[nodiscard]] screen get_cur_screen() const;
    [[nodiscard]] std::shared_ptr<tournament> get_cur_tournament() const;
    [[nodiscard]] std::shared_ptr<game_state> get_cur_game_state() const;
    [[nodiscard]] std::shared_ptr<sound_player> get_audio_player() const;
    vec2f get_view_center();

    void set_view_center(const vec2f &v);
    void set_cur_screen(screen s);
    void set_cur_game_state(std::shared_ptr<game_state> st);
    void create_audio_player();
};

extern client_state current_state;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CLIENT_H
