#include "../../include/screens/tournament_screens.h"
#include "../../include/tournament.h"
#include "../../include/ui_functions.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages {

void tournament_join_screen_init(tgui::Gui &gui) {
    auto tournament_join_screen_group = tgui::Group::create();

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress(
        [&] { show_screen(gui, screen::TOURNAMENT_CREATION, screen::TOURNAMENT_JOINING); });
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
    return_back_button->onPress([&] { show_screen(gui, screen::START_SCREEN, screen::TOURNAMENT_JOINING); });
    tournament_join_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{create_tournament_button, tournament_key_box,
                                           join_tournament_button, return_back_button};

    place_widgets(widgets);

    gui.add(tournament_join_screen_group, screen_id.at(screen::TOURNAMENT_JOINING));
    gui.get(screen_id.at(screen::TOURNAMENT_JOINING))->setVisible(false);
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
        [&] { show_screen(gui, screen::TOURNAMENT_MAIN, screen::TOURNAMENT_CREATION); });
    tournament_creation_screen_group->add(create_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&] { show_screen(gui, screen::TOURNAMENT_JOINING, screen::TOURNAMENT_CREATION); });
    tournament_creation_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{tournament_name_box, participants_number_box,
                                           create_tournament_button, return_back_button};

    place_widgets(widgets);

    gui.add(tournament_creation_screen_group, screen_id.at(screen::TOURNAMENT_CREATION));
    gui.get(screen_id.at(screen::TOURNAMENT_CREATION))->setVisible(false);
}

void tournament_screen_init(tgui::Gui &gui) {
    auto tournament_screen_group = tgui::Group::create();

    tgui::Label::Ptr tournament_name = tgui::Label::create(
        "Название турнира (TODO)");  // TODO: insert a valid name when logic is implemented
    tournament_name->setSize({"30%", "10%"});
    tournament_name->setTextSize(30);
    tournament_name->setPosition({"5%", "5%"});
    tournament_screen_group->add(tournament_name);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->setSize({"30%", "10%"});
    return_back_button->setPosition({"5%", "20%"});
    return_back_button->onPress(
        [&] { show_screen(gui, screen::TOURNAMENT_JOINING, screen::TOURNAMENT_MAIN); });
    tournament_screen_group->add(return_back_button);

    tgui::Grid::Ptr tournament_grid = tgui::Grid::create();
    tournament_grid->setPosition({"40%", "5%"});
    tournament_screen_group->add(tournament_grid, "tournament_grid");

    // std::vector<tgui::Widget::Ptr> widgets{return_back_button};
    // place_widgets(widgets);

    // TODO: remove sample tournament, when tournaments are implemented
    tournament t;
    t.add_participant("part1");
    t.add_participant("part2");
    t.add_participant("part3");
    t.add_participant("part4");
    t.add_participant("part5");
    t.add_participant("part6");
    t.add_participant("part7");
    t.add_participant("part8");
    t.add_participant("part9");
    t.add_participant("part10");
    t.add_result("part1", "part2");
    t.add_result("part3", "part2");
    t.add_result("part2", "part4");
    t.add_result("part2", "part1");
    t.update_grid(tournament_grid);
    // end sample tournament

    gui.add(tournament_screen_group, screen_id.at(screen::TOURNAMENT_MAIN));
    gui.get(screen_id.at(screen::TOURNAMENT_MAIN))->setVisible(false);
}

}  // namespace war_of_ages
