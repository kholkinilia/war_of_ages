#include "../include/ui_functions.h"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <string>
#include <vector>

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width, int height, int space) {
    /// Places widgets in the center of a screen in the order specified by their order in widgets vector.
    ///
    /// Width: width of each Widget in percent of screen width. (default = 40)
    /// Height: height of each Widget in percent of screen height. (default = 10)
    /// Space: a gap between each pair of adjacent Widgets in percent of screen height. (default = 5)
    ///
    /// Normally you don't want to specify width, height or space parameters, because it will lead to
    /// differences between screens.

    int length = static_cast<int>(widgets.size()) * (height + space) - space;
    int cur_y_pos = (100 - length) / 2;
    int x_pos = (100 - width) / 2;
    for (auto &w : widgets) {
        w->setSize(get_layout(width, height));
        w->setPosition(get_layout(x_pos, cur_y_pos));
        cur_y_pos += height + space;
    }
}

void show_screen(tgui::Gui &gui, war_of_ages::screen new_screen, war_of_ages::screen prev_screen) {
    gui.get(screen_id.at(prev_screen))->setVisible(false);
    gui.get(screen_id.at(new_screen))->setVisible(true);
    current_state.set_cur_screen(new_screen);
    if (current_state.get_cur_game_state() == nullptr && new_screen == screen::GAME_SCREEN) {
        current_state.set_cur_game_state(std::make_shared<game_state>());
    }
}

tgui::Layout2d get_layout(int width_percent, int height_percent) {
    return {tgui::String(std::to_string(width_percent) + "%"),
            tgui::String(std::to_string(height_percent) + "%")};
}

}  // namespace war_of_ages
