#ifndef WAR_OF_AGES_UI_FUNCTIONS_H
#define WAR_OF_AGES_UI_FUNCTIONS_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <vector>

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width = 40, int height = 10, int space = 5);

tgui::Layout2d get_layout(int width_percent, int height_percent);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_UI_FUNCTIONS_H
