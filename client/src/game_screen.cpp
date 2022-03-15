#include "../include/game_screen.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "../include/game_object_size_constants.h"
#include "../include/screen_defines.h"
#include "../include/sprite_supplier.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

const int BUTTON_SIZE = 70;
const int BUTTON_Y = 50;
const int DELTA_X = 96;
const int HP_SIZE = 400;
const int BAR_HEIGHT = 20;
const int BAR_Y = 216;

void game_screen_init(tgui::Gui &gui) {
    auto game_screen_group = tgui::Group::create();
    auto panel = tgui::ScrollablePanel::create();
    panel->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    auto background = tgui::Picture::create(
        *(sprite_supplier::get_instance().get_background_sprite(age_type::STONE).getTexture()));
    background->setSize(ROAD_WIDTH, BACKGROUND_HEIGHT);
    background->setPosition(0, 0);
    panel->add(background, screen_id.at(screen::GAME_SCREEN));

    auto road = tgui::Picture::create(
        *(sprite_supplier::get_instance().get_road_sprite(age_type::STONE).getTexture()));
    road->setSize(ROAD_WIDTH, ROAD_HEIGHT);
    road->setPosition(0, BACKGROUND_HEIGHT - ROAD_HEIGHT);
    panel->add(road, screen_id.at(screen::GAME_SCREEN));

    auto first_player_hp = tgui::ProgressBar::create();
    first_player_hp->setPosition(DELTA_X, BAR_Y);
    first_player_hp->setSize(HP_SIZE, BAR_HEIGHT);
    first_player_hp->setValue(80);
    auto first_player_hp_renderer = tgui::ProgressBarRenderer();
    first_player_hp_renderer.setFillColor({0, 255, 0, 255});
    first_player_hp->setRenderer(first_player_hp_renderer.getData());
    panel->add(first_player_hp);

    auto second_player_hp = tgui::ProgressBar::create();
    second_player_hp->setPosition(ROAD_WIDTH - HP_SIZE - DELTA_X, BAR_Y);
    second_player_hp->setSize(HP_SIZE, BAR_HEIGHT);
    second_player_hp->setValue(20);
    auto second_player_hp_renderer = tgui::ProgressBarRenderer();
    second_player_hp_renderer.setFillColor({255, 0, 0, 255});
    second_player_hp->setRenderer(second_player_hp_renderer.getData());
    panel->add(second_player_hp);

    auto units_group = tgui::Group::create();
    panel->add(units_group, "units_group");

    auto autobattle_button = tgui::BitmapButton::create();
    autobattle_button->setImage("../client/resources/pictures/autobattle.png");
    autobattle_button->setImageScaling(1.05);
    autobattle_button->setPosition(BACKGROUND_WIDTH - DELTA_X, BUTTON_Y);
    autobattle_button->setSize(BUTTON_SIZE, BUTTON_SIZE);
    autobattle_button->onPress([]() { std::cout << "AUTOBATTLE" << std::endl; });

    auto new_era_button = tgui::BitmapButton::create();
    new_era_button->setImage("../client/resources/pictures/new_era.jpg");
    new_era_button->setImageScaling(1.05);
    new_era_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 2, BUTTON_Y);
    new_era_button->setSize(BUTTON_SIZE, BUTTON_SIZE);
    new_era_button->onPress([]() { std::cout << "NEW ERA" << std::endl; });

    auto plus_place_cannon_button = tgui::BitmapButton::create();
    plus_place_cannon_button->setImage("../client/resources/pictures/plus_embrasure.jpg");
    plus_place_cannon_button->setImageScaling(1.05);
    plus_place_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y);
    plus_place_cannon_button->setSize(BUTTON_SIZE, BUTTON_SIZE);
    plus_place_cannon_button->onPress([]() { std::cout << "PLUS PLACE CANNON" << std::endl; });

    auto plus_cannon_button = tgui::BitmapButton::create();
    plus_cannon_button->setImage("../client/resources/pictures/plus_cannon.jpg");
    plus_cannon_button->setImageScaling(1.05);
    plus_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 4, BUTTON_Y);
    plus_cannon_button->setSize(BUTTON_SIZE, BUTTON_SIZE);
    plus_cannon_button->onPress([]() { std::cout << "PLUS CANNON" << std::endl; });

    auto plus_unit_button = tgui::BitmapButton::create();
    plus_unit_button->setImage("../client/resources/pictures/plus_unit.jpg");
    plus_unit_button->setImageScaling(1.05);
    plus_unit_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 5, BUTTON_Y);
    plus_unit_button->setSize(BUTTON_SIZE, BUTTON_SIZE);
    plus_unit_button->onPress([]() { std::cout << "PLUS UNIT" << std::endl; });

    auto pause_button = tgui::BitmapButton::create();
    pause_button->setImage("../client/resources/pictures/blue_settings_icon.png");
    pause_button->setImageScaling(1.05);
    pause_button->setPosition(DELTA_X, BUTTON_Y);
    pause_button->setSize(BUTTON_SIZE, BUTTON_SIZE);
    pause_button->onPress([&gui]() { show_screen(gui, screen::PAUSE, screen::GAME_SCREEN); });

    game_screen_group->add(panel, "panel");
    game_screen_group->add(autobattle_button);
    game_screen_group->add(new_era_button);
    game_screen_group->add(plus_place_cannon_button);
    game_screen_group->add(plus_cannon_button);
    game_screen_group->add(plus_unit_button);
    game_screen_group->add(pause_button);
    gui.add(game_screen_group, screen_id.at(screen::GAME_SCREEN));
    gui.get(screen_id.at(screen::GAME_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages