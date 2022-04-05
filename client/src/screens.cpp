#include "../include/screens.h"
#include <TGUI/Widgets/Label.hpp>
#include "../include/menu_background.h"

// Screen headers

#include "../include/end_game_screen.h"
#include "../include/game_screen.h"
#include "../include/multiplayer_screen.h"
#include "../include/pause_screen.h"
#include "../include/settings_screen.h"
#include "../include/sprite_printer.h"
#include "../include/start_screen.h"
#include "../include/tournament_screens.h"
#include "../include/ui_functions.h"
#include "../include/wait_screen.h"

namespace war_of_ages {

void screens_init(sf::View &v, tgui::Gui &gui) {
    // Widgets of every screen are defined here

    // Ilya's screens

    tournament_screen_init(gui);
    tournament_creation_screen_init(gui);
    tournament_join_screen_init(gui);

    // end Ilya's screens

    // Vakhtang's screens

    start_screen_init(gui);
    multiplayer_screen_init(gui);
    settings_screen_init(gui);

    // end Vakhtang's screens

    // Timur's screens

    opponent_waiting_screen_init(gui);
    game_screen_init(v, gui);
    end_game_screen_init(gui);
    pause_screen_init(v, gui);

    // end Timur's screens
}

void update_screens(tgui::Gui &gui, const client_state &state, sf::RenderWindow *window) {
    update_fps(gui.get("fps_label")->cast<tgui::Label>());
    if (state.get_cur_screen() != screen::GAME_SCREEN) {
        menu_background_handler::get_instance().print_background(*window, 1.f * clock() / CLOCKS_PER_SEC);
    }
    switch (state.get_cur_screen()) {
        case screen::TOURNAMENT_MAIN: {
            // TODO: uncomment, when tournament logic is implemented
            // state.get_cur_tournament()->update_grid(gui.get(screen_id.at(screen::TOURNAMENT_MAIN))
            //                                            ->cast<tgui::Group>()
            //                                            ->get("tournament_grid")
            //                                            ->cast<tgui::Grid>());
        }
        case screen::GAME_SCREEN: {
            if (state.get_cur_game_state() == nullptr)
                return;
            print(gui, window, state.get_cur_game_state());
        }
        default:
            break;
    }
}

}  // namespace war_of_ages
