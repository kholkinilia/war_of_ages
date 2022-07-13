#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "screen_handler.h"

namespace war_of_ages::client {

void screen_handler::wait_for_server_screen_init() {
    auto wait_screen_group = tgui::Group::create();

    tgui::Label::Ptr waiting_label =
        tgui::Label::create("Если вы это видите, то у вас проблемы с интернетом. Извиняйтесь.");
    waiting_label->setTextSize(30);
    waiting_label->getRenderer()->setTextColor(tgui::Color::White);
    wait_screen_group->add(waiting_label);

    tgui::Button::Ptr return_back_button = tgui::Button::create("В главное меню");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&] { screen_handler::instance().change_screen(screen_handler::screen_type::START_SCREEN); });
    wait_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets;
    widgets.push_back(waiting_label);
    widgets.push_back(return_back_button);

    place_widgets(widgets);

    m_gui.add(wait_screen_group, screen_id.at(screen_type::WAITING_FOR_SERVER));
    m_gui.get(screen_id.at(screen_type::WAITING_FOR_SERVER))->setVisible(false);
}

}  // namespace war_of_ages::client
