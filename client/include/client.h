#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include <memory>
#include "game_handler.h"
#include "screen_defines.h"
#include "sound_player.h"
#include "tournament.h"

namespace war_of_ages {

// Will be implemented to the end by Timur

struct client_state {
    enum class game_mode { SINGLE, MULTI };

private:
    std::string handle;
    screen cur_screen;
    std::shared_ptr<tournament> cur_tournament;
    std::shared_ptr<game_handler> cur_game;
    std::shared_ptr<sound_player> audio_player;
    vec2f view_center;

public:
    explicit client_state(std::string handle_ = "handle", screen cur_screen_ = screen::START_SCREEN);

    [[nodiscard]] std::string get_handle() const;
    [[nodiscard]] std::string get_cur_screen_id() const;
    [[nodiscard]] screen get_cur_screen() const;
    [[nodiscard]] std::shared_ptr<tournament> get_cur_tournament() const;
    [[nodiscard]] std::shared_ptr<game_state> get_cur_game_state() const;
    [[nodiscard]] std::shared_ptr<sound_player> get_audio_player() const;
    [[nodiscard]] vec2f get_view_center() const noexcept;
    [[nodiscard]] std::shared_ptr<game_handler> get_cur_game() const noexcept;

    void set_view_center(const vec2f &v);
    void set_cur_screen(screen s);

    void create_game(game_mode mode);
    void reset_game();
    void create_audio_player();
};

extern client_state current_state;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CLIENT_H
