#include "../include/screens.h"
#include <TGUI/Widgets/Label.hpp>

// Screen headers

#include "../include/screens/end_game_screen.h"
#include "../include/screens/game_screen.h"
#include "../include/screens/multiplayer_screen.h"
#include "../include/screens/settings_screen.h"
#include "../include/screens/start_screen.h"
#include "../include/screens/tournament_screens.h"
#include "../include/screens/wait_screen.h"
#include "../include/sfml_printer.h"
#include "../include/ui_functions.h"

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
    settings_screen_init(v, gui);

    // end Vakhtang's screens

    // Timur's screens

    opponent_waiting_screen_init(gui);
    game_screen_init(v, gui);
    end_game_screen_init(gui);

    // end Timur's screens
}

void update_screens(tgui::Gui &gui, const client_state &state, sf::RenderWindow *window) {
}

}  // namespace war_of_ages
