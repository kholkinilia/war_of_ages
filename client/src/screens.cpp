#include "../include/screens.h"
#include "../include/client.h"
#include "../include/screen_defines.h"

namespace war_of_ages {

void update_widgets(tgui::Gui &gui, const client_state &state) {
    switch (state.get_cur_screen()) {
        case screen::TOURNAMENT_MAIN: {
            state.get_cur_tournament()->update_grid(gui.get(screen_id.at(screen::TOURNAMENT_MAIN))
                                                        ->cast<tgui::Group>()
                                                        ->get("tournament_grid")
                                                        ->cast<tgui::Grid>());
        }
        case screen::GAME_SCREEN: {
            gui.get(screen_id.at(screen::GAME_SCREEN))
            ->cast<tgui::Group>()
            ->get(screen_id.at(screen::GAME_SCREEN))->setVisible(rand() % 2);
        }
        default:
            break;
    }
}

}  // namespace war_of_ages
