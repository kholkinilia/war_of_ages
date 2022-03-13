#ifndef WAR_OF_AGES_SCREENS_H
#define WAR_OF_AGES_SCREENS_H

#include "client.h"
#include "game_screen.h"
#include "multiplayer_screen.h"
#include "settings_screen.h"
#include "start_screen.h"
#include "tournament_screens.h"
#include "wait_screen.h"

namespace war_of_ages {

void update_widgets(tgui::Gui &gui, const client_state &state);
}

#endif  // WAR_OF_AGES_SCREENS_H
