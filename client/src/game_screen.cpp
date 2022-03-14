#include "../include/game_screen.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "../include/screen_defines.h"
#include "../include/sprite_supplier.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void game_screen_init(tgui::Gui &gui) {
    auto game_screen_group = tgui::Group::create();
    auto panel = tgui::ScrollablePanel::create();
    auto background = tgui::Picture::create(
        *(sprite_supplier::get_instance().get_background_sprite(age_type::STONE).getTexture()));
    background->setSize(1920 * 2, 1080);
    background->setPosition(0, 0);
    panel->add(background, screen_id.at(screen::GAME_SCREEN));
    panel->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);

    auto road = tgui::Picture::create(
        *(sprite_supplier::get_instance().get_road_sprite(age_type::STONE).getTexture()));
    road->setSize(1920 * 2, 200);
    road->setPosition(0, 880);
    panel->add(road, screen_id.at(screen::GAME_SCREEN));

    auto first_player_hp = tgui::ProgressBar::create();
    first_player_hp->setPosition(96, 216);
    first_player_hp->setSize(300, 20);
    first_player_hp->setValue(80);
    auto first_player_hp_renderer = tgui::ProgressBarRenderer();
    first_player_hp_renderer.setFillColor({0, 255, 0, 255});
    first_player_hp->setRenderer(first_player_hp_renderer.getData());
    panel->add(first_player_hp);

    auto second_player_hp = tgui::ProgressBar::create();
    second_player_hp->setPosition(3444, 216);
    second_player_hp->setSize(300, 20);
    second_player_hp->setValue(20);
    auto second_player_hp_renderer = tgui::ProgressBarRenderer();
    second_player_hp_renderer.setFillColor({255, 0, 0, 255});
    second_player_hp->setRenderer(second_player_hp_renderer.getData());
    panel->add(second_player_hp);

    auto autobattle_button = tgui::BitmapButton::create();
    autobattle_button->setImage("../client/resources/pictures/autobattle.png");
    autobattle_button->setImageScaling(1.05);
    autobattle_button->setPosition(1824, 50);
    autobattle_button->setSize(70, 70);
    autobattle_button->onPress([]() { std::cout << "AUTOBATTLE" << std::endl; });

    auto new_era_button = tgui::BitmapButton::create();
    new_era_button->setImage("../client/resources/pictures/new_era.jpg");
    new_era_button->setImageScaling(1.05);
    new_era_button->setPosition(1728, 50);
    new_era_button->setSize(70, 70);
    new_era_button->onPress([]() { std::cout << "NEW ERA" << std::endl; });

    auto plus_place_cannon_button = tgui::BitmapButton::create();
    plus_place_cannon_button->setImage("../client/resources/pictures/plus_embrasure.jpg");
    plus_place_cannon_button->setImageScaling(1.05);
    plus_place_cannon_button->setPosition(1632, 50);
    plus_place_cannon_button->setSize(70, 70);
    plus_place_cannon_button->onPress([]() { std::cout << "PLUS PLACE CANNON" << std::endl; });

    auto plus_cannon_button = tgui::BitmapButton::create();
    plus_cannon_button->setImage("../client/resources/pictures/plus_cannon.jpg");
    plus_cannon_button->setImageScaling(1.05);
    plus_cannon_button->setPosition(1536, 50);
    plus_cannon_button->setSize(70, 70);
    plus_cannon_button->onPress([]() { std::cout << "PLUS CANNON" << std::endl; });

    auto plus_unit_button = tgui::BitmapButton::create();
    plus_unit_button->setImage("../client/resources/pictures/plus_unit.jpg");
    plus_unit_button->setImageScaling(1.05);
    plus_unit_button->setPosition(1440, 50);
    plus_unit_button->setSize(70, 70);
    plus_unit_button->onPress([]() { std::cout << "PLUS UNIT" << std::endl; });

    auto pause_button = tgui::BitmapButton::create();
    pause_button->setImage("../client/resources/pictures/blue_settings_icon.png");
    pause_button->setImageScaling(1.05);
    pause_button->setPosition(50, 50);
    pause_button->setSize(70, 70);
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
}  // namespace war_of_ages