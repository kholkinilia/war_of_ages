#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include "../../include/bot_actions_receiver.h"
#include "../../include/client.h"
#include "../../include/screen_handler.h"
#include "../../include/sfml_printer.h"

namespace war_of_ages {

enum class action { BUY_UNIT, BUY_CANNON, SELL_CANNON };

static void setup_button(tgui::Button::Ptr &button, tgui::String name = "") {
    if (name != "") {
        button->getRenderer()->setTexture(tgui::String(std::move(name)));
    }
    button->getRenderer()->setBorders(0);
}

[[nodiscard]] static std::string to_string(age_type age) {
    switch (age) {
        case age_type::STONE:
            return "stone";
        case age_type::CASTLE:
            return "castle";
        case age_type::RENAISSANCE:
            return "renaissance";
        case age_type::MODERN:
            return "modern";
        case age_type::FUTURE:
            return "future";
    }
}

[[nodiscard]] static tgui::String get_filename(action a, int i, age_type age) noexcept {
    switch (a) {
        case action::BUY_UNIT:
            return tgui::String("../client/resources/game/units/" + to_string(age) + "/mini/") +
                   std::to_string(i + 1) + tgui::String(".png");
        case action::BUY_CANNON:
            return tgui::String("../client/resources/game/cannons/" + to_string(age) + "/level") +
                   std::to_string(i + 1) + tgui::String(".png");
        default:
            return tgui::String("../client/resources/pictures/") + std::to_string(i + 1) +
                   tgui::String(".png");
    }
}

static void setup_buttons_cluster(std::vector<tgui::Group::Ptr> &groups, action a) {
    static int k = 4;
    int n;
    switch (a) {
        case action::BUY_UNIT:
            n = UNITS_PER_AGE;
            break;
        case action::BUY_CANNON:
            n = CANNONS_PER_AGE;
            break;
        default:
            n = MAX_CANNON_SLOTS;
    }
    for (int i = 0; i < n; i++, k++) {
        groups[i] = tgui::Group::create();
        auto button = tgui::Button::create();
        setup_button(button, get_filename(a, i, age_type::STONE));
        button->setPosition(BACKGROUND_WIDTH - DELTA_X * k, BUTTON_Y);
        button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
        button->onPress([i, a]() {
            int slot = 0;
            auto cannons = current_state.get_cur_game_state()->snapshot_players().first.cannons;
            switch (a) {
                case action::BUY_UNIT:
                    current_state.get_cur_game()->append_action(0, std::make_unique<buy_unit_command>(i));
                    break;
                case action::BUY_CANNON:
                    for (int j = 0; j < cannons.size(); j++) {
                        auto c = cannons[j];
                        if (c.type() == cannon_type::NONE) {
                            slot = j;
                            break;
                        }
                    }
                    current_state.get_cur_game()->append_action(
                        0, std::make_unique<buy_cannon_command>(i, slot));
                    break;
                default:
                    current_state.get_cur_game()->append_action(0, std::make_unique<sell_cannon_command>(i));
            }
        });
        groups[i]->add(button, std::to_string(i));

        auto coin_image = tgui::Picture::create("../client/resources/pictures/coin.jpeg");
        coin_image->setPosition(BACKGROUND_WIDTH - DELTA_X * k, FPS_LABEL_HEIGHT);
        coin_image->setSize(COST_WIDTH, COST_HEIGHT);
        groups[i]->add(coin_image, "coin_image");

        auto coin_label = tgui::Label::create();
        coin_label->getRenderer()->setTextSize(0.6 * COST_HEIGHT);
        coin_label->setPosition(BACKGROUND_WIDTH - DELTA_X * k + COST_WIDTH, FPS_LABEL_HEIGHT + 3);
        switch (a) {
            case action::BUY_UNIT:
                coin_label->setText('-' + std::to_string(unit::get_stats(static_cast<unit_type>(i)).cost));
                break;
            case action::BUY_CANNON:
                coin_label->setText('-' +
                                    std::to_string(cannon::get_stats(static_cast<cannon_type>(i)).cost));
                break;
            default:
                break;
        }
        groups[i]->add(coin_label, "coin_label");
    }
}

void screen_handler::game_screen_init(sf::View &v) {
    auto game_screen_group = tgui::Group::create();

    auto autobattle_button = tgui::Button::create();
    setup_button(autobattle_button, "../client/resources/pictures/autobattle.png");
    autobattle_button->setPosition(BACKGROUND_WIDTH - DELTA_X, BUTTON_Y);
    autobattle_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    autobattle_button->onPress([]() {
        current_state.get_cur_game()->set_receiver(
            0, current_state.get_cur_game()->get_type(0) == game_handler::player_type::BOT
                   ? game_handler::player_type::PLAYER
                   : game_handler::player_type::BOT);
    });

    auto new_era_button = tgui::Button::create();
    setup_button(new_era_button, "../client/resources/pictures/new_era.jpg");
    new_era_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 2, BUTTON_Y);
    new_era_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new_era_button->onPress([&]() {
        for (int i = 0; i < UNITS_PER_AGE; i++) {
            m_gui.get(screen_handler::screen_id.at(screen_handler::instance().get_screen_type()))
                ->cast<tgui::Group>()
                ->get("unit_" + std::to_string(i))
                ->cast<tgui::Group>()
                ->get(std::to_string(i))
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTexture(tgui::String(std::move(get_filename(
                    action::BUY_UNIT, i, current_state.get_cur_game_state()->snapshot_players().first.age))));
        }
        for (int i = 0; i < CANNONS_PER_AGE; i++) {
            m_gui.get(screen_handler::screen_id.at(screen_handler::instance().get_screen_type()))
                ->cast<tgui::Group>()
                ->get("cannon_" + std::to_string(i))
                ->cast<tgui::Group>()
                ->get(std::to_string(i))
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTexture(tgui::String(std::move(
                    get_filename(action::BUY_CANNON, i,
                                 current_state.get_cur_game_state()->snapshot_players().first.age))));
        }
        current_state.get_cur_game()->append_action(0, std::make_unique<upgrade_age_command>());
    });

    auto plus_place_cannon_button = tgui::Button::create();
    setup_button(plus_place_cannon_button, "../client/resources/pictures/plus_embrasure.jpg");
    plus_place_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y);
    plus_place_cannon_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_place_cannon_button->onPress([]() {
        current_state.get_cur_game()->append_action(0, std::make_unique<buy_cannon_slot_command>());
    });
    auto plus_place_cannon_coin_image = tgui::Picture::create("../client/resources/pictures/coin.jpeg");
    plus_place_cannon_coin_image->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, FPS_LABEL_HEIGHT);
    plus_place_cannon_coin_image->setSize(COST_WIDTH, COST_HEIGHT);

    auto plus_place_cannon_coin_label = tgui::Label::create();
    plus_place_cannon_coin_label->getRenderer()->setTextSize(0.6 * COST_HEIGHT);
    plus_place_cannon_coin_label->setPosition(BACKGROUND_WIDTH - DELTA_X * 3 + COST_WIDTH,
                                              FPS_LABEL_HEIGHT + 3);

    std::vector<tgui::Group::Ptr> cannon_groups(CANNONS_PER_AGE);
    setup_buttons_cluster(cannon_groups, action::BUY_CANNON);
    for (int i = 0; i < CANNONS_PER_AGE; i++) {
        game_screen_group->add(cannon_groups[i], "cannon_" + std::to_string(i));
    }

    std::vector<tgui::Group::Ptr> unit_groups(UNITS_PER_AGE);
    setup_buttons_cluster(unit_groups, action::BUY_UNIT);
    for (int i = 0; i < UNITS_PER_AGE; i++) {
        game_screen_group->add(unit_groups[i], "unit_" + std::to_string(i));
    }

    std::vector<tgui::Group::Ptr> sell_cannon_groups(CANNONS_PER_AGE);
    setup_buttons_cluster(sell_cannon_groups, action::SELL_CANNON);
    for (int i = 0; i < CANNONS_PER_AGE; i++) {
        game_screen_group->add(sell_cannon_groups[i], "sell_cannon_" + std::to_string(i));
    }

    auto pause_button = tgui::Button::create();
    setup_button(pause_button, "../client/resources/pictures/settings_icon.png");
    pause_button->setPosition(0, FPS_LABEL_HEIGHT);
    pause_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    pause_button->onPress([&]() {
        m_gui.get("background_group")->setVisible(true);
        screen_handler::instance().change_screen(screen_handler::screen_type::SETTINGS);
    });

    auto ulta_button = tgui::Button::create();
    ulta_button->setText("ULTA");
    ulta_button->setTextSize(50);
    ulta_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y + BUTTON_HEIGHT + HP_HEIGHT);
    ulta_button->setSize(BUTTON_WIDTH * 4, BUTTON_HEIGHT);
    ulta_button->onPress(
        []() { current_state.get_cur_game()->append_action(0, std::make_unique<use_ult_command>()); });

    auto coin_image = tgui::Picture::create("../client/resources/pictures/coin.jpeg");
    coin_image->setPosition(BUTTON_WIDTH, FPS_LABEL_HEIGHT);
    coin_image->setSize(COIN_WIDTH, COIN_HEIGHT);

    auto coin_label = tgui::Label::create();
    coin_label->getRenderer()->setTextSize(0.75 * COIN_HEIGHT);
    coin_label->setPosition(BUTTON_WIDTH + COIN_WIDTH, FPS_LABEL_HEIGHT + 1.5 * COIN_HEIGHT / COST_HEIGHT);

    auto exp_image = tgui::Picture::create("../client/resources/pictures/exp.png");
    exp_image->setPosition(BUTTON_WIDTH, FPS_LABEL_HEIGHT + COIN_HEIGHT);
    exp_image->setSize(COIN_WIDTH, COIN_HEIGHT);

    auto exp_label = tgui::Label::create();
    exp_label->getRenderer()->setTextSize(0.75 * COIN_HEIGHT);
    exp_label->setPosition(BUTTON_WIDTH + COIN_WIDTH,
                           FPS_LABEL_HEIGHT + 1.5 * COIN_HEIGHT / COST_HEIGHT + COIN_HEIGHT);

    game_screen_group->add(autobattle_button);
    game_screen_group->add(new_era_button);
    game_screen_group->add(plus_place_cannon_button);
    game_screen_group->add(pause_button);
    game_screen_group->add(ulta_button);
    game_screen_group->add(coin_image);
    game_screen_group->add(coin_label, "coin_label");
    game_screen_group->add(exp_image);
    game_screen_group->add(exp_label, "exp_label");
    game_screen_group->add(plus_place_cannon_coin_image);
    game_screen_group->add(plus_place_cannon_coin_label, "plus_place_cannon_coin_label");
    m_gui.add(game_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages