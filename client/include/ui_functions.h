#ifndef WAR_OF_AGES_UI_FUNCTIONS_H
#define WAR_OF_AGES_UI_FUNCTIONS_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>
#include "screen_defines.h"

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width = 40, int height = 10, int space = 5);

void show_screen(tgui::Gui &gui, war_of_ages::screen new_screen, war_of_ages::screen prev_screen);

tgui::Layout2d get_layout(int width_percent, int height_percent);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UI_FUNCTIONS_H
