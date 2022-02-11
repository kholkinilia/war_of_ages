#include "../include/tournament_screens.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "../include/ui_functions.h"

namespace war_of_ages {

void tournament_join_screen_init(tgui::Gui &gui) {
    auto tournament_join_screen_group = tgui::Group::create();

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress(
        [&] { show_screen(gui, TOURNAMENT_CREATION_SCREEN, TOURNAMENT_JOIN_SCREEN); });
    tournament_join_screen_group->add(create_tournament_button);

    tgui::EditBox::Ptr tournament_key_box = tgui::EditBox::create();
    tournament_key_box->setInputValidator("[a-zA-Z0-9]{0,20}");
    tournament_key_box->setTextSize(30);
    tournament_key_box->setDefaultText("введите код доступа к турнуру");
    tournament_join_screen_group->add(tournament_key_box);

    tgui::Button::Ptr join_tournament_button = tgui::Button::create("Присоединиться");
    join_tournament_button->setTextSize(30);
    tournament_join_screen_group->add(join_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(exit, 0);  // TODO: set valid screen view function
    tournament_join_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{create_tournament_button, tournament_key_box,
                                           join_tournament_button, return_back_button};

    place_widgets(widgets);

    gui.add(tournament_join_screen_group, TOURNAMENT_JOIN_SCREEN);
    gui.get(TOURNAMENT_JOIN_SCREEN)->setVisible(false);
}

void tournament_creation_screen_init(tgui::Gui &gui) {
    auto tournament_creation_screen_group = tgui::Group::create();

    tgui::EditBox::Ptr tournament_name_box = tgui::EditBox::create();
    tournament_name_box->setInputValidator("[a-zA-Z0-9а-яА-Я ]{0,35}");
    tournament_name_box->setTextSize(30);
    tournament_name_box->setDefaultText("Введите название турнира");
    tournament_creation_screen_group->add(tournament_name_box);

    tgui::EditBox::Ptr participants_number_box = tgui::EditBox::create();
    participants_number_box->setInputValidator(tgui::EditBox::Validator::UInt);
    participants_number_box->setTextSize(30);
    participants_number_box->setDefaultText("Введите кол-во человек в турнире");
    tournament_creation_screen_group->add(participants_number_box);

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress(
        [&] { show_screen(gui, TOURNAMENT_SCREEN, TOURNAMENT_CREATION_SCREEN); });
    tournament_creation_screen_group->add(create_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&] { show_screen(gui, TOURNAMENT_JOIN_SCREEN, TOURNAMENT_CREATION_SCREEN); });
    tournament_creation_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{tournament_name_box, participants_number_box,
                                           create_tournament_button, return_back_button};

    place_widgets(widgets);

    gui.add(tournament_creation_screen_group, TOURNAMENT_CREATION_SCREEN);
    gui.get(TOURNAMENT_CREATION_SCREEN)->setVisible(false);
}

void tournament_screen_init(tgui::Gui &gui) {
    // TODO: implement to the end

    auto tournament_screen_group = tgui::Group::create();

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress([&] { show_screen(gui, TOURNAMENT_JOIN_SCREEN, TOURNAMENT_SCREEN); });
    tournament_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{return_back_button};

    place_widgets(widgets);

    gui.add(tournament_screen_group, TOURNAMENT_SCREEN);
    gui.get(TOURNAMENT_SCREEN)->setVisible(false);
}

} // namespace war_of_ages
