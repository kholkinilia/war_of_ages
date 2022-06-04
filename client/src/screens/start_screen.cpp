#include "../../include/screen_handler.h"
#include "../../include/single_player_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include "../../include/application.h"
#include "../../include/client.h"
#include "../../include/sfml_printer.h"
#include "../../include/sound_player.h"

namespace war_of_ages::client {
void screen_handler::start_screen_init() {
    auto start_screen_group = tgui::Group::create();

    tgui::Button::Ptr singleplayer_button = tgui::Button::create("Одиночная игра");
    singleplayer_button->setTextSize(30);
    singleplayer_button->onPress([&]() {
        application::instance().set_state(application::state::SINGLE_PLAYER_GAME);
        screen_handler::instance().change_screen(screen_handler::screen_type::GAME_SCREEN);
        sound_player::instance().change(sound_player::sound_type::LOBBY, sound_player::sound_type::BATTLE);
        single_player_handler::instance().start_game();
    });
    start_screen_group->add(singleplayer_button);

    tgui::Button::Ptr multiplayer_button = tgui::Button::create("Мультиплеер");
    multiplayer_button->setTextSize(30);
    multiplayer_button->onPress([&]() {
        if (!client::instance().get_is_authorized()) {
            screen_handler::instance().change_screen(screen_handler::screen_type::UNAUTHORIZED_SCREEN);
        } else {
            screen_handler::instance().change_screen(screen_handler::screen_type::MULTIPLAYER);
            application::instance().set_state(application::state::MULTIPLAYER);
        }
    });
    start_screen_group->add(multiplayer_button);

    tgui::Button::Ptr statistics_button = tgui::Button::create("Статистика");
    statistics_button->setTextSize(30);
    statistics_button->onPress([&]() {
        if (!client::instance().get_is_authorized()) {
            screen_handler::instance().change_screen(screen_handler::screen_type::UNAUTHORIZED_SCREEN);
        } else {
            screen_handler::instance().change_screen(screen_handler::screen_type::STATISTICS);
        }
    });
    start_screen_group->add(statistics_button);

    tgui::Button::Ptr exit_button = tgui::Button::create("Выйти из приложения");
    exit_button->setTextSize(30);
    exit_button->onPress([]() { sfml_printer::instance().get_window().close(); });
    start_screen_group->add(exit_button);

    auto login_button = tgui::Button::create("Войти в аккаунт");
    login_button->setTextSize(30);
    login_button->onPress([&]() {
        if (client::instance().get_is_authorized())
            return;
        application::instance().set_state(application::state::MULTIPLAYER);
        screen_handler::is_login = true;
        screen_handler::instance().change_screen(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION);
    });
    auto signup_button = tgui::Button::create("Зарегистрироваться");
    signup_button->setTextSize(30);
    signup_button->onPress([&]() {
        if (client::instance().get_is_authorized())
            return;
        application::instance().set_state(application::state::MULTIPLAYER);
        screen_handler::is_login = false;
        screen_handler::instance().change_screen(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION);
    });
    auto sign_out_button = tgui::Button::create("Выйти из аккаунта");
    sign_out_button->setTextSize(30);
    sign_out_button->onPress([&]() {
        if (!client::instance().get_is_authorized())
            return;

        client::instance().set_is_authorized(false);
        client::instance().disconnect();
        client::instance().set_handle("");
        client::instance().set_password("");
        message<messages_type> msg;
        msg.header.id = messages_type::AUTH_LOGOUT;
        client::instance().send_message(msg);
    });

    start_screen_group->add(login_button);
    start_screen_group->add(signup_button);
    start_screen_group->add(sign_out_button);

    std::vector<tgui::Widget::Ptr> widgets{singleplayer_button, multiplayer_button, statistics_button,
                                           login_button,        signup_button,      sign_out_button,
                                           exit_button};
    place_widgets(widgets);

    tgui::Button::Ptr settings_screen_button = tgui::Button::create();
    auto &theme = screen_handler::instance().get_theme_buttons();
    settings_screen_button->setRenderer(theme.getRenderer("settings_icon"));
    settings_screen_button->getRenderer()->setBorders(0);
    settings_screen_button->setSize("4.5%", "8%");
    settings_screen_button->setPosition("90%", "5%");
    settings_screen_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::SETTINGS); });
    start_screen_group->add(settings_screen_button);

    m_gui.add(start_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::START_SCREEN));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::START_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages::client