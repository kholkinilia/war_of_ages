#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include <memory>
#include "../../game_logic/include/game_state.h"
#include "screen_defines.h"
#include "tournament.h"

namespace war_of_ages {

// Will be implemented to the end by Timur

struct client_state {
private:
    std::string handle;
    screen cur_screen;
    std::shared_ptr<tournament> cur_tournament;
    std::shared_ptr<game_state> cur_game_state;
    vec2f view_center;
    std::vector<std::vector<std::unique_ptr<game_command>>> player_actions;

public:
    explicit client_state(std::string handle_ = "handle", screen cur_screen_ = screen::START_SCREEN);

    [[nodiscard]] std::string get_handle() const;
    [[nodiscard]] std::string get_cur_screen_id() const;
    [[nodiscard]] screen get_cur_screen() const;
    [[nodiscard]] std::shared_ptr<tournament> get_cur_tournament() const;
    [[nodiscard]] std::shared_ptr<game_state> get_cur_game_state() const;
    [[nodiscard]] vec2f get_view_center() const noexcept;
    [[nodiscard]] const std::vector<std::vector<std::unique_ptr<game_command>>> &get_player_actions() const;

    void set_view_center(const vec2f &v);
    void set_cur_screen(screen s);
    void set_cur_game_state(std::shared_ptr<game_state> st);

    void add_action(int player, std::unique_ptr<game_command> cmd);
    void clear_actions();
};

extern client_state current_state;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CLIENT_H
