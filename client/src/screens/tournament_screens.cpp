#include "messages_type.h"
#include "../../include/client.h"
#include "../../include/screen_handler.h"
#include "../../include/tournament_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages::client {

void screen_handler::tournament_join_screen_init() {
    auto tournament_join_screen_group = tgui::Group::create();

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress(
        [&] { screen_handler::instance().change_screen(screen_handler::screen_type::TOURNAMENT_CREATION); });
    tournament_join_screen_group->add(create_tournament_button);

    tgui::EditBox::Ptr tournament_key_box = tgui::EditBox::create();
    tournament_key_box->setInputValidator("[a-zA-Z0-9]{0,20}");
    tournament_key_box->setTextSize(30);
    tournament_key_box->setDefaultText("введите код доступа к турниру");
    tournament_join_screen_group->add(tournament_key_box, "tournament_key_box");

    tgui::Button::Ptr join_tournament_button = tgui::Button::create("Присоединиться");
    join_tournament_button->setTextSize(30);
    join_tournament_button->onPress([&] {
        message<messages_type> msg;
        msg.header.id = messages_type::TOURNAMENT_JOIN;
        std::string key = static_cast<std::string>(m_gui.get(screen_id.at(screen_type::TOURNAMENT_JOINING))
                                                       ->cast<tgui::Group>()
                                                       ->get("tournament_key_box")
                                                       ->cast<tgui::EditBox>()
                                                       ->getText());
        msg << key;
        client::instance().send_message(msg);

        change_screen(screen_type::WAITING_FOR_SERVER);
    });
    tournament_join_screen_group->add(join_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&] { screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU); });
    tournament_join_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{create_tournament_button, tournament_key_box,
                                           join_tournament_button, return_back_button};

    place_widgets(widgets);

    m_gui.add(tournament_join_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_JOINING));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_JOINING))
        ->setVisible(false);
}

void screen_handler::tournament_creation_screen_init() {
    auto tournament_creation_screen_group = tgui::Group::create();

    tgui::EditBox::Ptr tournament_name_box = tgui::EditBox::create();
    tournament_name_box->setInputValidator("[a-zA-Z0-9а-яА-Я ]{0,35}");
    tournament_name_box->setTextSize(30);
    tournament_name_box->setDefaultText("Введите название турнира");
    tournament_creation_screen_group->add(tournament_name_box, "tournament_name_box");

    tgui::EditBox::Ptr participants_number_box = tgui::EditBox::create();
    participants_number_box->setInputValidator(tgui::EditBox::Validator::UInt);
    participants_number_box->setTextSize(30);
    participants_number_box->setDefaultText("Введите кол-во человек в турнире");
    tournament_creation_screen_group->add(participants_number_box, "participant_numebr_box");

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress([&] {
        message<messages_type> msg;
        msg.header.id = messages_type::TOURNAMENT_CREATE;
        std::string name = static_cast<std::string>(m_gui.get(screen_id.at(screen_type::TOURNAMENT_CREATION))
                                                        ->cast<tgui::Group>()
                                                        ->get("tournament_name_box")
                                                        ->cast<tgui::EditBox>()
                                                        ->getText());
        msg.insert_container(name);

        client::instance().send_message(msg);

        change_screen(screen_type::WAITING_FOR_SERVER);
    });
    tournament_creation_screen_group->add(create_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&] { screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU); });
    tournament_creation_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{tournament_name_box, participants_number_box,
                                           create_tournament_button, return_back_button};

    place_widgets(widgets);

    m_gui.add(tournament_creation_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_CREATION));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_CREATION))
        ->setVisible(false);
}

void screen_handler::tournament_screen_init() {
    auto tournament_screen_group = tgui::Group::create();

    tgui::Label::Ptr tournament_name = tgui::Label::create("Название турнира (текст меняется не тут)");
    tournament_name->setSize({"30%", "10%"});
    tournament_name->setTextSize(30);
    tournament_name->setPosition({"5%", "5%"});
    tournament_screen_group->add(tournament_name, "tournament_name");

    tgui::Label::Ptr tournament_key = tgui::Label::create("Ключ турнира:");
    tournament_key->setSize({"30%", "10%"});
    tournament_key->setTextSize(30);
    tournament_key->setPosition({"5%", "20%"});
    tournament_screen_group->add(tournament_key, "tournament_key_label");

    tgui::EditBox::Ptr tournament_key_box = tgui::EditBox::create();
    tournament_key_box->setSize({"30%", "10%"});
    tournament_key_box->setTextSize(30);
    tournament_key_box->setPosition({"5%", "35%"});
    tournament_screen_group->add(tournament_key_box, "tournament_key_box");

    tgui::Button::Ptr return_back_button = tgui::Button::create("Покинуть турнир");
    return_back_button->setTextSize(30);
    return_back_button->setSize({"30%", "10%"});
    return_back_button->setPosition({"5%", "50%"});
    return_back_button->onPress([&] {
        message<messages_type> msg;
        msg.header.id = messages_type::TOURNAMENT_LEAVE;
        client::instance().send_message(msg);

        screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU);
    });
    tournament_screen_group->add(return_back_button);

    tgui::Grid::Ptr tournament_grid = tgui::Grid::create();
    tournament_grid->setPosition({"40%", "5%"});
    tournament_screen_group->add(tournament_grid, "tournament_grid");

    m_gui.add(tournament_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_MAIN));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_MAIN))->setVisible(false);
}

}  // namespace war_of_ages::client
