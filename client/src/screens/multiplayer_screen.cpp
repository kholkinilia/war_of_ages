#include "../../include/client.h"
#include "../../include/screen_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>

namespace war_of_ages::client {
void screen_handler::multiplayer_screen_init() {
    auto multiplayer_screen_group = tgui::Group::create();

    tgui::Button::Ptr random_game_button = tgui::Button::create("Случайная игра");
    random_game_button->setTextSize(30);
    random_game_button->onPress([&]() {
        message<messages_type> msg;
        msg.header.id = messages_type::RANDOMGAME_JOIN;
        client::instance().send_message(msg);
        change_screen(screen_type::WAIT_OPPONENT);
    });
    multiplayer_screen_group->add(random_game_button);

    tgui::EditBox::Ptr room_id_editbox = tgui::EditBox::create();
    room_id_editbox->setTextSize(30);
    room_id_editbox->setDefaultText("Номер комнаты:");
    room_id_editbox->setInputValidator(tgui::EditBox::Validator::UInt);
    multiplayer_screen_group->add(room_id_editbox, "room_id_editbox");

    tgui::Button::Ptr join_room_button = tgui::Button::create("Присоединиться");
    join_room_button->setTextSize(30);
    join_room_button->onPress([this]() {
        message<messages_type> msg;
        msg.header.id = messages_type::ROOM_JOIN;
        std::string room_id =
            static_cast<std::string>(m_gui.get<tgui::Group>(screen_id.at(screen_type::MULTIPLAYER))
                                         ->get<tgui::EditBox>("room_id_editbox")
                                         ->getText());
        msg << room_id;
        client::instance().send_message(msg);
    });
    multiplayer_screen_group->add(join_room_button);

    tgui::Button::Ptr tournament_button = tgui::Button::create("Турниры");
    tournament_button->setTextSize(30);
    tournament_button->onPress(
        [&]() { change_screen(screen_type::TOURNAMENT_JOINING); });
    multiplayer_screen_group->add(tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&]() { change_screen(screen_type::PREVIOUS_MENU); });
    multiplayer_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{random_game_button, room_id_editbox, join_room_button,
                                           tournament_button, return_back_button};

    place_widgets(widgets);
    m_gui.add(multiplayer_screen_group,
              screen_id.at(screen_type::MULTIPLAYER));
    m_gui.get(screen_id.at(screen_type::MULTIPLAYER))->setVisible(false);

    auto room_joining_failed_msg =
        tgui::Button::create("Не удалось подключиться к комнате\nВероятно, она уже занята");
    room_joining_failed_msg->setTextSize(30);
    room_joining_failed_msg->getRenderer()->setTextColor(tgui::Color::Red);
    room_joining_failed_msg->onPress([this]() {
        m_gui.get<tgui::Group>(screen_id.at(get_screen_type()))->setEnabled(true);
        m_gui.get<tgui::Button>("room_joining_failed_msg")->setVisible(false);
    });
    room_joining_failed_msg->setVisible(false);
    widgets = {room_joining_failed_msg};
    place_widgets(widgets);
    m_gui.add(room_joining_failed_msg, "room_joining_failed_msg");
}
}  // namespace war_of_ages::client