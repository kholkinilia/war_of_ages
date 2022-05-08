#include "../../include/screen_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Picture.hpp>

namespace war_of_ages::client {
void screen_handler::background_init() {
    auto background_group = tgui::Group::create();

    auto background_texture = tgui::Texture("../client/resources/pictures/fullHD_kittens.jpg");
    auto background_picture = tgui::Picture::create(background_texture);
    background_group->add(background_picture);

    m_gui.add(background_group, "background_group");
    m_gui.get("background_group")->setVisible(true);
}
}  // namespace war_of_ages::client