#ifndef WAR_OF_AGES_UI_FUNCTIONS_H
#define WAR_OF_AGES_UI_FUNCTIONS_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <vector>
#include "screen_handler.h"

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width = 40, int height = 10, int space = 5);

void show_screen(tgui::Gui &gui, screen_handler::screen_type new_screen, screen_handler::screen_type prev_screen);

tgui::Layout2d get_layout(int width_percent, int height_percent);

void update_fps(std::shared_ptr<tgui::Label> label);

void setup_fps(tgui::Gui &gui);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UI_FUNCTIONS_H
