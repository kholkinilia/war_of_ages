#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "../../include/screen_handler.h"

namespace war_of_ages::client {

void screen_handler::wait_for_server_screen_init() {
    auto wait_screen_group = tgui::Group::create();

    tgui::Label::Ptr waiting_label =
        tgui::Label::create("Если вы это видите, то у вас проблемы с интернетом. Извиняйтесь.");
    waiting_label->setTextSize(30);
    waiting_label->getRenderer()->setTextColor(tgui::Color::White);
    wait_screen_group->add(waiting_label);

    std::vector<tgui::Widget::Ptr> widgets;
    widgets.push_back(waiting_label);

    place_widgets(widgets);

    m_gui.add(wait_screen_group, screen_id.at(screen_type::WAITING_FOR_SERVER));
    m_gui.get(screen_id.at(screen_type::WAITING_FOR_SERVER));
}

}  // namespace war_of_ages::client
