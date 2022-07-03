#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include "../../include/client.h"
#include "../../include/room_handler.h"
#include "../../include/screen_handler.h"

namespace war_of_ages::client {
void screen_handler::room_screen_init() {
    auto room_screen_group = tgui::Group::create();

    tgui::Texture rate_texture("../../client/resources/pictures/rate.png");

    auto readiness_button = tgui::Button::create();
    readiness_button->setPosition({"37%", "45%"});
    readiness_button->setSize({"10%", "10%"});
    readiness_button->onPress([]() {
        room_handler::instance().switch_my_status();
        message<messages_type> msg;
        msg.header.id = messages_type::ROOM_SWITCH_STATUS;
        client::instance().send_message(msg);
    });

    auto enemy_readiness_picture = tgui::Picture::create();
    enemy_readiness_picture->setPosition({"53%", "45%"});
    enemy_readiness_picture->setSize({"10%", "10%"});

    auto my_handle = tgui::Label::create();
    my_handle->getRenderer()->setTextColor(tgui::Color::Green);
    my_handle->setTextSize(35);
    my_handle->setPosition({"2%", "2%"});

    auto my_rate_value = tgui::Label::create();
    my_rate_value->setTextSize(40);
    my_rate_value->getRenderer()->setTextColor(tgui::Color::Yellow);
    my_rate_value->setPosition({"6%", "8.5%"});

    auto my_rate_picture = tgui::Picture::create(rate_texture);
    my_rate_picture->setPosition({"2%", "9%"});
    my_rate_picture->setSize({"3%", "4%"});

    auto enemy_handle = tgui::Label::create();
    enemy_handle->getRenderer()->setTextColor(tgui::Color::Red);
    enemy_handle->setTextSize(35);
    enemy_handle->setPosition({"98%", "2%"});
    enemy_handle->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);

    auto enemy_rate_value = tgui::Label::create();
    enemy_rate_value->setTextSize(40);
    enemy_rate_value->getRenderer()->setTextColor(tgui::Color::Yellow);
    enemy_rate_value->setPosition({"84%", "8.5%"});
    enemy_rate_value->setSize({"10%", "10%"});
    enemy_rate_value->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);

    auto enemy_rate_picture = tgui::Picture::create(rate_texture);
    enemy_rate_picture->setPosition({"95%", "9%"});
    enemy_rate_picture->setSize({"3%", "4%"});

    auto left_button = tgui::Button::create("Покинуть комнату");
    left_button->setTextSize(30);
    left_button->setPosition({"80%", "90%"});
    left_button->setSize({"20%", "10%"});
    left_button->onPress([this]() {
        message<messages_type> msg;
        msg.header.id = messages_type::ROOM_LEAVE;
        client::instance().send_message(msg);
    });

    auto me_group = tgui::Group::create();
    me_group->add(readiness_button, "my_readiness");
    me_group->add(my_handle, "my_handle");
    me_group->add(my_rate_value, "my_rate_value");
    me_group->add(my_rate_picture, "my_rate_picture");

    auto enemy_group = tgui::Group::create();
    enemy_group->add(enemy_readiness_picture, "enemy_readiness");
    enemy_group->add(enemy_handle, "enemy_handle");
    enemy_group->add(enemy_rate_value, "enemy_rate_value");
    enemy_group->add(enemy_rate_picture, "enemy_rate_picture");

    room_screen_group->add(me_group, "me_group");
    room_screen_group->add(enemy_group, "enemy_group");

    room_screen_group->add(left_button, "leave_button");

    m_gui.add(room_screen_group, screen_id.at(screen_type::ROOM_SCREEN));
    m_gui.get(screen_id.at(screen_type::ROOM_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages::client
