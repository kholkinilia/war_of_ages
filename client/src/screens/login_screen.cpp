#include <fstream>
#include "../../include/application.h"
#include "../../include/client.h"
#include "../../include/screen_handler.h"
#include "messages_type.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages::client {

void screen_handler::unauthorized_screen_init() {
    auto unauthorized_screen_group = tgui::Group::create();
    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");

    auto unauthorized_label = tgui::Label::create();
    unauthorized_label->getRenderer()->setTextSize(35);
    unauthorized_label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    unauthorized_label->getRenderer()->setTextColor(tgui::Color::White);
    unauthorized_label->setText("Вы не вошли в сеть");
    unauthorized_screen_group->add(unauthorized_label, "unauthorized_label");

    std::vector<tgui::Widget::Ptr> widgets = {unauthorized_label};
    place_widgets(widgets);

    auto return_button = tgui::Button::create("Назад");
    return_button->setRenderer(black_theme.getRenderer("Button"));
    return_button->setTextSize(30);
    return_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU); });
    return_button->setPosition("30%", "86%");
    return_button->setSize("40%", "10%");
    unauthorized_screen_group->add(return_button, "return_button");

    m_gui.add(unauthorized_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::UNAUTHORIZED_SCREEN));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::UNAUTHORIZED_SCREEN))
        ->setVisible(false);
}

void screen_handler::login_screen_init() {
    auto login_screen_group = tgui::Group::create();

    auto login_box = tgui::EditBox::create();
    login_box->setInputValidator("[a-zA-Z0-9]{0,20}");
    login_box->setTextSize(30);
    login_box->setDefaultText("Логин");
    login_screen_group->add(login_box, "login_box");

    auto password_box = tgui::EditBox::create();
    password_box->setInputValidator("[a-zA-Z0-9]{0,20}");
    password_box->setTextSize(30);
    password_box->setDefaultText("Пароль");
    login_screen_group->add(password_box, "password_box");

    auto login_or_authorization = [](bool is_login) {
        client::instance().clear_messages();

        client::instance().set_handle(
            static_cast<std::string>(screen_handler::instance()
                                         .get_gui()
                                         .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                                         ->cast<tgui::Group>()
                                         ->get("login_box")
                                         ->cast<tgui::EditBox>()
                                         ->getText()));
        client::instance().set_password(
            static_cast<std::string>(screen_handler::instance()
                                         .get_gui()
                                         .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                                         ->cast<tgui::Group>()
                                         ->get("password_box")
                                         ->cast<tgui::EditBox>()
                                         ->getText()));
        client::instance().clear_messages();
        if (!client::instance().is_connected()) {
            if (client::instance().connect(client::instance().get_server_ip(),
                                           client::instance().get_server_port())) {
                client::instance().login_or_authorize(is_login);
            }
        } else {
            client::instance().login_or_authorize(is_login);
        }
    };

    auto send_button = tgui::Button::create("Войти");
    send_button->setTextSize(30);
    send_button->onPress([&]() { login_or_authorization(true); });
    login_screen_group->add(send_button, "send_button");

    auto signup_button = tgui::Button::create("Зарегистрироваться");
    signup_button->setTextSize(30);
    signup_button->onPress([&]() { login_or_authorization(false); });

    auto return_button = tgui::Button::create("Назад");
    return_button->setTextSize(30);
    return_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::START_SCREEN); });
    login_screen_group->add(return_button, "return_button");

    std::vector<tgui::Widget::Ptr> widgets = {login_box, password_box, send_button, signup_button,
                                              return_button};
    place_widgets(widgets);

    m_gui.add(login_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION))
        ->setVisible(false);
}
}  // namespace war_of_ages::client
