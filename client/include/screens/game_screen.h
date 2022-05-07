#ifndef WAR_OF_AGES_GAME_SCREEN_H
#define WAR_OF_AGES_GAME_SCREEN_H

#include <TGUI/Backend/SFML-Graphics.hpp>

namespace war_of_ages {

enum class action { BUY_UNIT, BUY_CANNON, SELL_CANNON };

void game_screen_init(sf::View &v, tgui::Gui &gui);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_SCREEN_H
