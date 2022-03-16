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

    auto units_group = tgui::Group::create();
    units_group->setSize(ROAD_WIDTH, BACKGROUND_HEIGHT);
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
    plus_unit_button->onPress([]() {
        std::vector<std::unique_ptr<game_command>> v;
        v.push_back(std::make_unique<buy_unit_command>(0));
        current_state.get_cur_game_state()->update(v, {}, 0.001);
    });

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