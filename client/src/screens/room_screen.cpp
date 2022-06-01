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

    tgui::Texture rate_texture("../client/resources/pictures/rate.png");

    auto readiness_button = tgui::Button::create();
    readiness_button->setPosition("37%", "45%");
    readiness_button->setSize("10%", "10%");
    readiness_button->onPress([]() {
        room_handler::instance().switch_my_status();
        message<messages_type> msg;
        msg.header.id = messages_type::ROOM_CHANGE_STATUS;
        client::instance().send_message(msg);
    });

    auto enemy_readiness_picture = tgui::Picture::create(rate_texture);
    enemy_readiness_picture->setPosition("53%", "45%");
    enemy_readiness_picture->setSize("10%", "10%");

    auto my_handle = tgui::Label::create();
    my_handle->setPosition("2%", "2%");
    my_handle->setSize("10%", "4%");

    auto my_rate_value = tgui::Label::create();
    my_rate_value->setPosition("6%", "7%");
    my_rate_value->setSize("6%", "3%");

    auto my_rate_picture = tgui::Picture::create();
    my_rate_picture->setPosition("2%", "6%");
    my_rate_picture->setSize("3%", "4%");

    auto enemy_handle = tgui::Label::create();
    enemy_handle->setPosition("98%", "2%");
    enemy_handle->setSize("6%", "3%");
    enemy_handle->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);

    auto enemy_rate_value = tgui::Label::create();
    enemy_rate_value->setPosition("94%", "7%");
    enemy_rate_value->setSize("3%", "4%");
    enemy_rate_value->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);

    auto enemy_rate_picture = tgui::Picture::create(rate_texture);
    enemy_rate_picture->setPosition("98%", "6%");
    enemy_rate_picture->setSize("3%", "4%");

    auto left_button = tgui::Button::create("Покинуть комнату");
    left_button->setPosition("80%", "80%");
    left_button->setSize("20%", "20%");
    left_button->onPress([this]() {
        change_screen(screen_type::PREVIOUS_MENU);
        message<messages_type> msg;
        msg.header.id = messages_type::ROOM_LEAVE;
        client::instance().send_message(msg);
    });

    room_screen_group->add(readiness_button, "my_readiness");
    room_screen_group->add(enemy_readiness_picture, "enemy_readiness");
    room_screen_group->add(my_handle, "my_handle");
    room_screen_group->add(my_rate_value, "my_rate_value");
    room_screen_group->add(my_rate_picture, "my_rate_picture");
    room_screen_group->add(enemy_handle, "enemy_handle");
    room_screen_group->add(enemy_rate_value, "enemy_rate_value");
    room_screen_group->add(enemy_rate_picture, "enemy_rate_picture");

    m_gui.add(room_screen_group, screen_id.at(screen_type::ROOM_SCREEN));
}
}  // namespace war_of_ages::client
