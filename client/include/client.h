#ifndef WAR_OF_AGES_CLIENT_H
#define WAR_OF_AGES_CLIENT_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include <memory>
#include "../include/tournament_handler.h"
#include "screen_handler.h"
#include "single_player_handler.h"

namespace war_of_ages {

// Will be implemented to the end by Timur

struct client_state {
private:
    std::string handle;
    std::shared_ptr<tournament_handler> cur_tournament;

public:
    explicit client_state(std::string handle_ = "handle");

    [[nodiscard]] std::string get_handle() const;
    [[nodiscard]] std::shared_ptr<tournament_handler> get_cur_tournament() const;
};

extern client_state current_state;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_CLIENT_H
