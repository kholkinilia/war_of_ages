#include "../include/game_screen.h"
#include <iostream>
#include "../include/game_object_size_constants.h"
#include "../include/screen_defines.h"
#include "../include/sprite_supplier.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

void game_screen_init(tgui::Gui &gui) {
    auto game_screen_group = tgui::Group::create();

    auto autobattle_button = tgui::BitmapButton::create();
    autobattle_button->setImage("../client/resources/pictures/autobattle.png");
    autobattle_button->setImageScaling(1.05);
    autobattle_button->setPosition(BACKGROUND_WIDTH - DELTA_X, BUTTON_Y);
    autobattle_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    autobattle_button->onPress([]() { std::cout << "AUTOBATTLE" << std::endl; });

    auto new_era_button = tgui::BitmapButton::create();
    new_era_button->setImage("../client/resources/pictures/new_era.jpg");
    new_era_button->setImageScaling(1.05);
    new_era_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 2, BUTTON_Y);
    new_era_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new_era_button->onPress([]() { std::cout << "NEW ERA" << std::endl; });

    auto plus_place_cannon_button = tgui::BitmapButton::create();
    plus_place_cannon_button->setImage("../client/resources/pictures/plus_embrasure.jpg");
    plus_place_cannon_button->setImageScaling(1.05);
    plus_place_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y);
    plus_place_cannon_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_place_cannon_button->onPress([]() { std::cout << "PLUS PLACE CANNON" << std::endl; });

    auto plus_cannon_button = tgui::BitmapButton::create();
    plus_cannon_button->setImage("../client/resources/pictures/plus_cannon.jpg");
    plus_cannon_button->setImageScaling(1.05);
    plus_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 4, BUTTON_Y);
    plus_cannon_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_cannon_button->onPress([]() { std::cout << "PLUS CANNON" << std::endl; });

    auto plus_peasant_button = tgui::BitmapButton::create();
    plus_peasant_button->setImage(
        *(sprite_supplier::get_instance()
              .get_unit_sprite(unit_type::PEASANT, sprite_supplier::player_side::RIGHT)
              .getTexture()));
    plus_peasant_button->setImageScaling(1);
    plus_peasant_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 5, BUTTON_Y);
    plus_peasant_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_peasant_button->onPress([]() {
        std::vector<std::unique_ptr<game_command>> v;
        v.push_back(std::make_unique<buy_unit_command>(2));
        current_state.get_cur_game_state()->update(v, {}, 1.f * clock() / CLOCKS_PER_SEC);
    });

    auto plus_archer_button = tgui::BitmapButton::create();
    plus_archer_button->setImage(
        *(sprite_supplier::get_instance()
              .get_unit_sprite(unit_type::ARCHER, sprite_supplier::player_side::RIGHT)
              .getTexture()));
    plus_archer_button->setImageScaling(1);
    plus_archer_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 6, BUTTON_Y);
    plus_archer_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_archer_button->onPress([]() {
        std::vector<std::unique_ptr<game_command>> v;
        v.push_back(std::make_unique<buy_unit_command>(1));
        current_state.get_cur_game_state()->update(v, {}, 1.f * clock() / CLOCKS_PER_SEC);
    });
    auto plus_chariot_button = tgui::BitmapButton::create();
    plus_chariot_button->setImage(
        *(sprite_supplier::get_instance()
              .get_unit_sprite(unit_type::CHARIOT, sprite_supplier::player_side::RIGHT)
              .getTexture()));
    plus_chariot_button->setImageScaling(1);
    plus_chariot_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 7, BUTTON_Y);
    plus_chariot_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_chariot_button->onPress([]() {
        std::vector<std::unique_ptr<game_command>> v;
        v.push_back(std::make_unique<buy_unit_command>(2));
        current_state.get_cur_game_state()->update(v, {}, 1.f * clock() / CLOCKS_PER_SEC);
    });

    auto pause_button = tgui::BitmapButton::create();
    pause_button->setImage("../client/resources/pictures/blue_settings_icon.png");
    pause_button->setImageScaling(1.05);
    pause_button->setPosition(DELTA_X, BUTTON_Y);
    pause_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    pause_button->onPress([&gui]() { show_screen(gui, screen::PAUSE, screen::GAME_SCREEN); });

    auto ulta_button = tgui::BitmapButton::create();
    ulta_button->setText("ULTA");
    ulta_button->setTextSize(50);
    ulta_button->setImageScaling(1.05);
    ulta_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 4, BUTTON_Y + BUTTON_HEIGHT + HP_HEIGHT);
    ulta_button->setSize(BUTTON_WIDTH * 5, BUTTON_HEIGHT);
    ulta_button->onPress([&gui]() { std::cout << "ULTA" << std::endl; });

    game_screen_group->add(autobattle_button);
    game_screen_group->add(new_era_button);
    game_screen_group->add(plus_place_cannon_button);
    game_screen_group->add(plus_cannon_button);
    game_screen_group->add(plus_peasant_button);
    game_screen_group->add(plus_archer_button);
    game_screen_group->add(plus_chariot_button);
    game_screen_group->add(pause_button);
    game_screen_group->add(ulta_button);
    gui.add(game_screen_group, screen_id.at(screen::GAME_SCREEN));
    gui.get(screen_id.at(screen::GAME_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages