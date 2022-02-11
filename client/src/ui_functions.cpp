#include "../include/ui_functions.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width, int height, int space) {
    int number = static_cast<int>(widgets.size());
    int length = number * (height + space) - space;
    int cur_y_pos = (100 - length) / 2;
    int x_pos = (100 - width) / 2;
    for (auto &w : widgets) {
        w->setSize(tgui::String(std::to_string(width) + "%"), tgui::String(std::to_string(height) + "%"));
        w->setPosition(tgui::String(std::to_string(x_pos) + "%"),
                       tgui::String(std::to_string(cur_y_pos) + "%"));
        cur_y_pos += height + space;
    }
}

void show_screen(tgui::Gui &gui,
                 const tgui::String &new_screen_group,
                 const tgui::String &prev_screen_group) {
    gui.get(prev_screen_group)->setVisible(false);
    gui.get(new_screen_group)->setVisible(true);
}

}  // namespace war_of_ages
