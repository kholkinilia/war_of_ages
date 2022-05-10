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
        screen_handler::instance().change_screen(screen_handler::screen_type::WAIT_OPPONENT);
    });
    multiplayer_screen_group->add(random_game_button);

    tgui::EditBox::Ptr room_id_editbox = tgui::EditBox::create();
    room_id_editbox->setTextSize(30);
    room_id_editbox->setDefaultText("Номер комнаты:");
    room_id_editbox->setInputValidator(tgui::EditBox::Validator::UInt);
    multiplayer_screen_group->add(room_id_editbox);

    tgui::Button::Ptr join_room_button = tgui::Button::create("Присоединиться");
    join_room_button->setTextSize(30);
    // TODO: onPress: try join to room with given id (key)
    join_room_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::WAIT_OPPONENT); });
    multiplayer_screen_group->add(join_room_button);

    tgui::Button::Ptr tournament_button = tgui::Button::create("Турниры");
    tournament_button->setTextSize(30);
    tournament_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::TOURNAMENT_JOINING); });
    multiplayer_screen_group->add(tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU); });
    multiplayer_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{random_game_button, room_id_editbox, join_room_button,
                                           tournament_button, return_back_button};

    place_widgets(widgets);
    m_gui.add(multiplayer_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::MULTIPLAYER));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::MULTIPLAYER))->setVisible(false);
}
}  // namespace war_of_ages::client