#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include <memory>
#include "game_handler.h"
#include "screen_handler.h"
#include "tournament.h"

namespace war_of_ages {

// Will be implemented to the end by Timur

struct client_state {
    enum class game_mode { SINGLE, MULTI };

private:
    std::string handle;
    std::shared_ptr<tournament> cur_tournament;
    std::shared_ptr<game_handler> cur_game;

public:
    explicit client_state(std::string handle_ = "handle");

    [[nodiscard]] std::string get_handle() const;
    [[nodiscard]] std::shared_ptr<tournament> get_cur_tournament() const;
    [[nodiscard]] std::shared_ptr<game_state> get_cur_game_state() const;
    [[nodiscard]] std::shared_ptr<game_handler> get_cur_game() const noexcept;

    void create_game(game_mode mode);
    void reset_game();
};

extern client_state current_state;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CLIENT_H
