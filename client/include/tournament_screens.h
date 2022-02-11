#ifndef WAR_OF_AGES_TOURNAMENT_SCREENS_H
#define WAR_OF_AGES_TOURNAMENT_SCREENS_H

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "screen_defines.h"

namespace war_of_ages {

void tournament_join_screen_init(tgui::Gui &gui);

void tournament_creation_screen_init(tgui::Gui &gui);

void tournament_screen_init(tgui::Gui &gui);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_SCREENS_H
