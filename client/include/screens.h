#ifndef WAR_OF_AGES_SCREENS_H
#define WAR_OF_AGES_SCREENS_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include "client.h"

namespace war_of_ages {

void screens_init(sf::View &v, tgui::Gui &gui);

void update_screens(tgui::Gui &gui, const client_state &state, sf::RenderWindow *window);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SCREENS_H
