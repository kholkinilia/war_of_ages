#include "../include/game_screen.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

#include <iostream>

namespace war_of_ages {
void game_screen_init(tgui::Gui &gui) {
    auto game_screen_group = tgui::Group::create();
    auto panel = tgui::ScrollablePanel::create();
    auto background = tgui::Picture::create("../client/resources/pictures/background.png");
    background->setSize(tgui::String("200%"), tgui::String("150%"));
    background->setPosition(tgui::String("0%"), tgui::String("-50%"));
    panel->add(background, screen_id.at(screen::GAME_SCREEN));
    panel->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);

    auto first_player_hp = tgui::ProgressBar::create();
    first_player_hp->setPosition(tgui::String("5%"), tgui::String("20%"));
    first_player_hp->setSize(tgui::String("20%"), 20);
    first_player_hp->setValue(80);
    auto first_player_hp_renderer = tgui::ProgressBarRenderer();
    first_player_hp_renderer.setFillColor({0, 255, 0, 255});
    first_player_hp->setRenderer(first_player_hp_renderer.getData());
    panel->add(first_player_hp);

    auto second_player_hp = tgui::ProgressBar::create();
    second_player_hp->setPosition(tgui::String("175%"), tgui::String("20%"));
    second_player_hp->setSize(tgui::String("20%"), 20);
    second_player_hp->setValue(20);
    auto second_player_hp_renderer = tgui::ProgressBarRenderer();
    second_player_hp_renderer.setFillColor({255, 0, 0, 255});
    second_player_hp->setRenderer(second_player_hp_renderer.getData());
    panel->add(second_player_hp);

    auto autobattle_button = tgui::BitmapButton::create();
    autobattle_button->setImage("../client/resources/pictures/autobattle.png");
    autobattle_button->setImageScaling(1.05);
    autobattle_button->setPosition(tgui::String("95%"), tgui::String("5%"));
    autobattle_button->setSize(70, 70);
    autobattle_button->onPress([]() {
        std::cout << "AUTOBATTLE" << std::endl;
    });

    auto new_era_button = tgui::BitmapButton::create();
    new_era_button->setImage("../client/resources/pictures/new_era.jpg");
    new_era_button->setImageScaling(1.05);
    new_era_button->setPosition(tgui::String("90%"), tgui::String("5%"));
    new_era_button->setSize(tgui::String(70), tgui::String(70));
    new_era_button->onPress([]() {
        std::cout << "NEW ERA" << std::endl;
    });

    auto plus_place_cannon_button = tgui::BitmapButton::create();
    plus_place_cannon_button->setImage("../client/resources/pictures/plus_embrasure.jpg");
    plus_place_cannon_button->setImageScaling(1.05);
    plus_place_cannon_button->setPosition(tgui::String("85%"), tgui::String("5%"));
    plus_place_cannon_button->setSize(tgui::String(70), tgui::String(70));
    plus_place_cannon_button->onPress([]() {
        std::cout << "PLUS PLACE CANNON" << std::endl;
    });

    auto plus_cannon_button = tgui::BitmapButton::create();
    plus_cannon_button->setImage("../client/resources/pictures/plus_cannon.jpg");
    plus_cannon_button->setImageScaling(1.05);
    plus_cannon_button->setPosition(tgui::String("80%"), tgui::String("5%"));
    plus_cannon_button->setSize(tgui::String(70), tgui::String(70));
    plus_cannon_button->onPress([]() {
        std::cout << "PLUS CANNON" << std::endl;
    });

    auto plus_unit_button = tgui::BitmapButton::create();
    plus_unit_button->setImage("../client/resources/pictures/plus_unit.jpg");
    plus_unit_button->setImageScaling(1.05);
    plus_unit_button->setPosition(tgui::String("75%"), tgui::String("5%"));
    plus_unit_button->setSize(tgui::String(70), tgui::String(70));
    plus_unit_button->onPress([]() {
        std::cout << "PLUS UNIT" << std::endl;
    });

    auto pause_button = tgui::BitmapButton::create();
    pause_button->setImage("../client/resources/pictures/blue_settings_icon.png");
    pause_button->setImageScaling(1.05);
    pause_button->setPosition(tgui::String("3%"), tgui::String("5%"));
    pause_button->setSize(tgui::String(70), tgui::String(70));
    pause_button->onPress(
        [&gui]() { show_screen(gui, screen_id.at(screen::PAUSE), screen_id.at(screen::GAME_SCREEN)); });

    game_screen_group->add(panel, screen_id.at(screen::GAME_SCREEN));
    game_screen_group->add(autobattle_button, screen_id.at(screen::GAME_SCREEN));
    game_screen_group->add(new_era_button, screen_id.at(screen::GAME_SCREEN));
    game_screen_group->add(plus_place_cannon_button, screen_id.at(screen::GAME_SCREEN));
    game_screen_group->add(plus_cannon_button, screen_id.at(screen::GAME_SCREEN));
    game_screen_group->add(plus_unit_button, screen_id.at(screen::GAME_SCREEN));
    game_screen_group->add(pause_button, screen_id.at(screen::GAME_SCREEN));
    gui.add(game_screen_group, screen_id.at(screen::GAME_SCREEN));
    gui.get(screen_id.at(screen::GAME_SCREEN))->setVisible(false);
}
}