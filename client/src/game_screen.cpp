#include "../include/game_screen.h"
#include <iostream>
#include "../include/game_object_size_constants.h"
#include "../include/screen_defines.h"
#include "../include/sprite_supplier.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

void setup_button(tgui::BitmapButton::Ptr button) {
    button->getRenderer()->setBorders(0);
    button->setImageScaling(1);
    button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);
    button->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
}

void game_screen_init(sf::View &v, tgui::Gui &gui) {
    auto game_screen_group = tgui::Group::create();

    auto autobattle_button = tgui::BitmapButton::create();
    autobattle_button->setImage("../client/resources/pictures/autobattle.png");
    setup_button(autobattle_button);
    autobattle_button->setPosition(BACKGROUND_WIDTH - DELTA_X, BUTTON_Y);
    autobattle_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    autobattle_button->onPress([]() { std::cout << "AUTOBATTLE" << std::endl; });

    auto new_era_button = tgui::BitmapButton::create();
    new_era_button->setImage("../client/resources/pictures/new_era.jpg");
    setup_button(new_era_button);
    new_era_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 2, BUTTON_Y);
    new_era_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new_era_button->onPress([]() { std::cout << "NEW ERA" << std::endl; });

    auto plus_place_cannon_button = tgui::BitmapButton::create();
    plus_place_cannon_button->setImage("../client/resources/pictures/plus_embrasure.jpg");
    setup_button(plus_place_cannon_button);
    plus_place_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y);
    plus_place_cannon_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_place_cannon_button->onPress([]() { std::cout << "PLUS PLACE CANNON" << std::endl; });

    auto plus_cannon_button = tgui::BitmapButton::create();
    plus_cannon_button->setImage("../client/resources/pictures/plus_cannon.jpg");
    setup_button(plus_cannon_button);
    plus_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 4, BUTTON_Y);
    plus_cannon_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_cannon_button->onPress([]() { std::cout << "PLUS CANNON" << std::endl; });

    std::vector<tgui::Group::Ptr> groups(UNITS_PER_AGE);
    for (int i = 0; i < UNITS_PER_AGE; i++) {
        groups[i] = tgui::Group::create();
        auto button = tgui::BitmapButton::create();
        button->setImage(tgui::String("../client/resources/game/units/stone/mini/") +
                         to_string(static_cast<unit_type>(i)) + tgui::String(".png"));
        setup_button(button);
        button->setPosition(BACKGROUND_WIDTH - DELTA_X * 5 - DELTA_X * i, BUTTON_Y);
        button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
        button->onPress([i]() {
            std::vector<std::unique_ptr<game_command>> v;
            v.push_back(std::make_unique<buy_unit_command>(i));
            current_state.get_cur_game_state()->update(v, {}, 1.f * clock() / CLOCKS_PER_SEC);
        });
        groups[i]->add(button);

        auto coin_image = tgui::Picture::create("../client/resources/pictures/coin.jpeg");
        coin_image->setPosition(BACKGROUND_WIDTH - DELTA_X * 5 - DELTA_X * i, FPS_LABEL_HEIGHT);
        coin_image->setSize(COST_WIDTH, COST_HEIGHT);
        groups[i]->add(coin_image);

        auto coin_label = tgui::Label::create();
        coin_label->getRenderer()->setTextSize(0.75 * COST_HEIGHT);
        coin_label->setPosition(BACKGROUND_WIDTH - DELTA_X * 5 - DELTA_X * i + COST_WIDTH, FPS_LABEL_HEIGHT);
        coin_label->setText(std::to_string(unit::get_stats(static_cast<unit_type>(i)).cost));
        groups[i]->add(coin_label);

        game_screen_group->add(groups[i]);
    }

    auto pause_button = tgui::BitmapButton::create();
    pause_button->setImage("../client/resources/pictures/blue_settings_icon.png");
    setup_button(pause_button);
    pause_button->setPosition(0, FPS_LABEL_HEIGHT);
    pause_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    pause_button->onPress([&gui, &v]() {
        current_state.set_view_center(v.getCenter());
        show_screen(gui, screen::PAUSE, screen::GAME_SCREEN);
    });

    auto ulta_button = tgui::BitmapButton::create();
    ulta_button->setText("ULTA");
    ulta_button->setTextSize(50);
    ulta_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 4, BUTTON_Y + BUTTON_HEIGHT + HP_HEIGHT);
    ulta_button->setSize(BUTTON_WIDTH * 5, BUTTON_HEIGHT);
    ulta_button->onPress([]() {
        std::vector<std::unique_ptr<game_command>> v;
        v.push_back(std::make_unique<use_ult_command>());
        current_state.get_cur_game_state()->update(v, {}, 1.f * clock() / CLOCKS_PER_SEC);
    });

    auto coin_image = tgui::Picture::create("../client/resources/pictures/coin.jpeg");
    coin_image->setPosition(BUTTON_WIDTH, FPS_LABEL_HEIGHT);
    coin_image->setSize(COIN_WIDTH, COIN_HEIGHT);

    auto coin_label = tgui::Label::create();
    coin_label->getRenderer()->setTextSize(0.75 * COIN_HEIGHT);
    coin_label->setPosition(BUTTON_WIDTH + COIN_WIDTH, FPS_LABEL_HEIGHT);

    game_screen_group->add(autobattle_button);
    game_screen_group->add(new_era_button);
    game_screen_group->add(plus_place_cannon_button);
    game_screen_group->add(plus_cannon_button);
    game_screen_group->add(pause_button);
    game_screen_group->add(ulta_button);
    game_screen_group->add(coin_image);
    game_screen_group->add(coin_label, "coin_label");
    gui.add(game_screen_group, screen_id.at(screen::GAME_SCREEN));
    gui.get(screen_id.at(screen::GAME_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages