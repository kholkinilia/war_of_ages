#ifndef WAR_OF_AGES_SPRITE_PRINTER_H
#define WAR_OF_AGES_SPRITE_PRINTER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../../game_logic/include/game_state.h"

namespace war_of_ages {

void print(tgui::Gui &gui, sf::RenderWindow *window, const std::shared_ptr<game_state> &state);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SPRITE_PRINTER_H
