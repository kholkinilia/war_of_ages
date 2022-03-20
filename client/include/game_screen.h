#ifndef WAR_OF_AGES_GAME_SCREEN_H
#define WAR_OF_AGES_GAME_SCREEN_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/BitmapButton.hpp>

namespace war_of_ages {
void game_screen_init(sf::View &v, tgui::Gui &gui);

void setup_button(tgui::BitmapButton::Ptr button);
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_SCREEN_H
