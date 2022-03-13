#include "../include/wait_screen.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

void opponent_waiting_screen_init(tgui::Gui &gui) {
    auto opponent_waiting_screen_group = tgui::Group::create();
    sf::Texture t;
    t.loadFromFile("../client/resources/pictures/loading.png");
    auto loading = tgui::Picture::create(t);
    opponent_waiting_screen_group->add(loading, "image");
    std::vector<tgui::Widget::Ptr> widgets = {loading};
    place_widgets(widgets);
    gui.add(opponent_waiting_screen_group, screen_id.at(screen::WAIT_OPPONENT));
    gui.get(screen_id.at(screen::WAIT_OPPONENT))->setVisible(false);
}

}