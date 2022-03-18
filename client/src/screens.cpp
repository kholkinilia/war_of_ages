#include "../include/screens.h"
#include "../include/client.h"
#include "../include/game_object_size_constants.h"
#include "../include/screen_defines.h"
#include "../include/sprite_supplier.h"
#include "../include/sprite_printer.h"

namespace war_of_ages {

void update_screens(tgui::Gui &gui, const client_state &state, sf::RenderWindow *window) {
    switch (state.get_cur_screen()) {
        case screen::TOURNAMENT_MAIN: {
            state.get_cur_tournament()->update_grid(gui.get(screen_id.at(screen::TOURNAMENT_MAIN))
                                                        ->cast<tgui::Group>()
                                                        ->get("tournament_grid")
                                                        ->cast<tgui::Grid>());
        }
        case screen::GAME_SCREEN: {
            if (state.get_cur_game_state() == nullptr)
                return;
            sprite_printer s;
            s.print(window, state.get_cur_game_state());
        }
        default:
            break;
    }
}

}  // namespace war_of_ages
