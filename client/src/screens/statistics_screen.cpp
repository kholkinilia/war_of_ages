#include "../../include/client.h"
#include "../../include/screen_handler.h"
#include "messages_type.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages::client {

void screen_handler::statistics_screen_init() {
    auto statistics_screen_group = tgui::Group::create();

    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");
    auto return_button = tgui::Button::create("В главное меню");
    return_button->setRenderer(black_theme.getRenderer("Button"));
    return_button->setTextSize(30);
    return_button->onPress([&]() {
        screen_handler::instance().change_screen(screen_handler::screen_type::START_SCREEN);
    });
    return_button->setPosition("30%", "86%");
    return_button->setSize("40%", "10%");
    statistics_screen_group->add(return_button, "return_button");
    m_gui.add(statistics_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::STATISTICS));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::STATISTICS))->setVisible(false);
}
}  // namespace war_of_ages::client
