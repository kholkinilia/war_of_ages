#include "../../include/screens/wait_screen.h"
#include "../../include/ui_functions.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Picture.hpp>

namespace war_of_ages {

void opponent_waiting_screen_init(tgui::Gui &gui) {
    auto opponent_waiting_screen_group = tgui::Group::create();
    auto loading = tgui::Picture::create("../client/resources/pictures/loading.png");
    opponent_waiting_screen_group->add(loading, "image");

    auto return_button = tgui::Button::create("В главное меню");
    return_button->setTextSize(30);
    return_button->onPress([&gui]() { show_screen(gui, screen::START_SCREEN, screen::WAIT_OPPONENT); });
    opponent_waiting_screen_group->add(return_button);

    std::vector<tgui::Widget::Ptr> widgets = {loading, return_button};
    place_widgets(widgets);
    gui.add(opponent_waiting_screen_group, screen_id.at(screen::WAIT_OPPONENT));
    gui.get(screen_id.at(screen::WAIT_OPPONENT))->setVisible(false);
}

}  // namespace war_of_ages