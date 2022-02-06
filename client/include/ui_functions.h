#ifndef WAR_OF_AGES_UI_FUNCTIONS_H
#define WAR_OF_AGES_UI_FUNCTIONS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width = 40, int height = 10, int space = 5);

void show_screen(tgui::Gui& gui, const tgui::String& new_screen_group, const tgui::String& prev_screen_group);
}

#endif //WAR_OF_AGES_UI_FUNCTIONS_H
