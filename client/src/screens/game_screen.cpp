#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <unordered_map>
#include "../../include/application.h"
#include "../../include/bot_actions_supplier.h"
#include "../../include/client.h"
#include "../../include/multiplayer_snapshots_handler.h"
#include "../../include/player_actions_supplier.h"
#include "../../include/screen_handler.h"
#include "../../include/sfml_printer.h"
#include "../../include/single_player_handler.h"

namespace war_of_ages::client {

static void setup_button(tgui::Button::Ptr &button, std::string name = "") {
    auto &theme = screen_handler::instance().get_theme_buttons();

    if (!name.empty()) {
        button->setRenderer(theme.getRenderer(std::move(name)));
    }
    button->getRenderer()->setBorders(0);
}

static const std::unordered_map<age_type, std::string> age_to_string = {
    {age_type::STONE, "stone"},
    {age_type::CASTLE, "castle"},
    // {age_type::RENAISSANCE, "renaissance"},
    // {age_type::MODERN, "modern"},
    // {age_type::FUTURE, "future"},
};

[[nodiscard]] std::string get_renderer(action a, int i, age_type age) noexcept {
    switch (a) {
        case action::BUY_UNIT:
            return "unit_" + age_to_string.at(age) + "_mini_" + std::to_string(i + 1);
//            return tgui::String("../client/resources/game/units/" + age_to_string.at(age) + "/mini/") +
//                   std::to_string(i + 1) + tgui::String(".png");
        case action::BUY_CANNON:
            return "cannon_" + age_to_string.at(age) + "_mini_" + std::to_string(i + 1);
//            return tgui::String("../client/resources/game/cannons/" + age_to_string.at(age) + "/level") +
//                   std::to_string(i + 1) + tgui::String(".png");
        case action::SELL_CANNON:
            return "sell_cannon_" + std::to_string(i + 1);
//            return tgui::String("../client/resources/pictures/") + std::to_string(i + 1) +
//                   tgui::String(".png");
        default:
            assert(!"Unreachable code!");
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
        case action::SELL_CANNON:
            n = MAX_CANNON_SLOTS;
            break;
        default:
            assert(!"Unreachable code!");
    }
    for (int i = 0; i < n; i++, k++) {
        groups[i] = tgui::Group::create();
        auto button = tgui::Button::create();
        setup_button(button, get_renderer(a, i, age_type::STONE));
        button->setPosition(BACKGROUND_WIDTH - DELTA_X * k, BUTTON_Y);
        button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
        button->onPress([i, a]() {
            int slot = 0;
            switch (application::instance().get_state()) {
                case application::state::SINGLE_PLAYER_GAME: {
                    switch (a) {
                        case action::BUY_UNIT:
                            player_actions_supplier::instance().add_action(
                                std::make_unique<buy_unit_command>(i));
                            break;
                        case action::BUY_CANNON: {
                            /* FIXME: do not get snapshot one more time */
                            auto cannons = single_player_handler::instance().get_snapshot().first.cannons;
                            for (int j = 0; j < cannons.size(); j++) {
                                auto c = cannons[j];
                                if (c.type() == cannon_type::NONE) {
                                    slot = j;
                                    break;
                                }
                            }
                            player_actions_supplier::instance().add_action(
                                std::make_unique<buy_cannon_command>(i, slot));
                            break;
                        }
                        case action::SELL_CANNON:
                            player_actions_supplier::instance().add_action(
                                std::make_unique<sell_cannon_command>(i));
                            break;
                        default:
                            assert(!"Unreachable code!");
                    }
                    break;
                }
                case application::state::MULTIPLAYER: {
                    message<messages_type> msg;
                    switch (a) {
                        case action::BUY_UNIT: {
                            msg.header.id = messages_type::GAME_BUY_UNIT;
                            msg << static_cast<std::uint8_t>(i);
                            break;
                        }
                        case action::BUY_CANNON: {
                            /* FIXME: do not get snapshot one more time */
                            auto cannons =
                                multiplayer_snapshots_handler::instance().get_snapshots().first.cannons;
                            for (int j = 0; j < cannons.size(); j++) {
                                auto c = cannons[j];
                                if (c.type() == cannon_type::NONE) {
                                    slot = j;
                                    break;
                                }
                            }
                            msg.header.id = messages_type::GAME_BUY_CANNON;
                            msg << static_cast<std::uint8_t>(i) << static_cast<std::uint8_t>(slot);
                            break;
                        }
                        case action::SELL_CANNON: {
                            msg.header.id = messages_type::GAME_SELL_CANNON;
                            msg << static_cast<std::uint8_t>(i);
                            break;
                        }
                        default:
                            assert(!"Unreachable code!");
                    }
                    client::instance().send_message(msg);
                    break;
                }
                default:
                    assert(!"Unreachable code!");
            }
        });
        groups[i]->add(button, std::to_string(i));

        auto coin_image = tgui::Picture::create("../client/resources/pictures/dollar.png");
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
            case action::SELL_CANNON:
                break;
            default:
                assert(!"Unreachable code!");
        }
        groups[i]->add(coin_label, "coin_label");
    }
}

void screen_handler::game_screen_init() {
    auto game_screen_group = tgui::Group::create();

    auto autobattle_button = tgui::Button::create();
    setup_button(autobattle_button, "logo");
    autobattle_button->setPosition(BACKGROUND_WIDTH - DELTA_X, BUTTON_Y);
    autobattle_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    autobattle_button->onPress([]() {
        single_player_handler::instance().change_player_type(
            single_player_handler::instance().get_type() == single_player_handler::player_type::PLAYER
                ? single_player_handler::player_type::BOT
                : single_player_handler::player_type::PLAYER);
    });

    auto new_era_button = tgui::Button::create();
    setup_button(new_era_button, "new_era");
    new_era_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 2, BUTTON_Y);
    new_era_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new_era_button->onPress([&]() {
        switch (application::instance().get_state()) {
            case application::state::SINGLE_PLAYER_GAME:
                player_actions_supplier::instance().add_action(std::make_unique<upgrade_age_command>());
                break;
            case application::state::MULTIPLAYER: {
                message<messages_type> msg;
                msg.header.id = messages_type::GAME_UPGRADE_AGE;
                client::instance().send_message(msg);
                break;
            }
            default:
                assert(!"Unreachable code!");
        }
    });

    auto plus_place_cannon_button = tgui::Button::create();
    setup_button(plus_place_cannon_button, "plus_embrasure");
    plus_place_cannon_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y);
    plus_place_cannon_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    plus_place_cannon_button->onPress([]() {
        switch (application::instance().get_state()) {
            case application::state::SINGLE_PLAYER_GAME:
                player_actions_supplier::instance().add_action(std::make_unique<buy_cannon_slot_command>());
                break;
            case application::state::MULTIPLAYER: {
                message<messages_type> msg;
                msg.header.id = messages_type::GAME_BUY_CANNON_SLOT;
                client::instance().send_message(msg);
                break;
            }
            default:
                assert(!"Unreachable code!");
        }
    });
    auto plus_place_cannon_coin_image = tgui::Picture::create("../client/resources/pictures/dollar.png");
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
    setup_button(pause_button, "settings_icon");
    pause_button->setPosition(0, FPS_LABEL_HEIGHT);
    pause_button->setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    pause_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::SETTINGS); });

    auto ulta_button = tgui::Button::create();
    ulta_button->setText("ULTA");
    ulta_button->setTextSize(50);
    ulta_button->setPosition(BACKGROUND_WIDTH - DELTA_X * 3, BUTTON_Y + BUTTON_HEIGHT + HP_HEIGHT);
    ulta_button->setSize(BUTTON_WIDTH * 4, BUTTON_HEIGHT);
    ulta_button->onPress([]() {
        switch (application::instance().get_state()) {
            case application::state::SINGLE_PLAYER_GAME:
                player_actions_supplier::instance().add_action(std::make_unique<use_ult_command>());
                break;
            case application::state::MULTIPLAYER: {
                message<messages_type> msg;
                msg.header.id = messages_type::GAME_USE_ULT;
                client::instance().send_message(msg);
                break;
            }
            default:
                assert(!"Unreachable code!");
        }
    });

    auto coin_image = tgui::Picture::create("../client/resources/pictures/dollar.png");
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

    // FIXME: get rid of literal constants
    auto enemy_handle_label = tgui::Label::create();
    enemy_handle_label->getRenderer()->setTextSize(20);
    enemy_handle_label->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    enemy_handle_label->getRenderer()->setTextColor(tgui::Color::Red);
    enemy_handle_label->setPosition(BACKGROUND_WIDTH - 200, BUTTON_HEIGHT + 200);

    game_screen_group->add(autobattle_button);
    game_screen_group->add(new_era_button);
    game_screen_group->add(plus_place_cannon_button);
    game_screen_group->add(pause_button);
    game_screen_group->add(ulta_button);
    game_screen_group->add(coin_image);
    game_screen_group->add(coin_label, "coin_label");
    game_screen_group->add(exp_image);
    game_screen_group->add(exp_label, "exp_label");
    game_screen_group->add(enemy_handle_label, "enemy_handle_label");
    game_screen_group->add(plus_place_cannon_coin_image);
    game_screen_group->add(plus_place_cannon_coin_label, "plus_place_cannon_coin_label");
    m_gui.add(game_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages::client