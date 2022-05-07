#include "../../include/screens/multiplayer_screen.h"
#include "../../include/screen_handler.h"
#include "../../include/ui_functions.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>

namespace war_of_ages {
void multiplayer_screen_init(tgui::Gui &gui) {
    auto multiplayer_screen_group = tgui::Group::create();

    tgui::Button::Ptr random_game_button = tgui::Button::create("Случайная игра");
    random_game_button->setTextSize(30);
    // TODO: onPress: search game & switch to waiting screen if success
    random_game_button->onPress(
        [&gui]() { screen_handler::instance().change_screen(screen_handler::screen_type::WAIT_OPPONENT); });
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
        [&gui]() { screen_handler::instance().change_screen(screen_handler::screen_type::WAIT_OPPONENT); });
    multiplayer_screen_group->add(join_room_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&gui]() { screen_handler::instance().change_screen(screen_handler::screen_type::START_SCREEN); });
    multiplayer_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{random_game_button, room_id_editbox, join_room_button,
                                           return_back_button};

    place_widgets(widgets);
    gui.add(multiplayer_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::MULTIPLAYER));
    gui.get(screen_handler::screen_id.at(screen_handler::screen_type::MULTIPLAYER))->setVisible(false);
}
}  // namespace war_of_ages