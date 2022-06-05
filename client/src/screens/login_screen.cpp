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

void screen_handler::signout_screen_init() {
    auto signout_screen_group = tgui::Group::create();
    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");

    auto authorized_label = tgui::Label::create();
    authorized_label->getRenderer()->setTextSize(35);
    authorized_label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    authorized_label->getRenderer()->setTextColor(tgui::Color::White);
    authorized_label->setText("Вы вошли в сеть, как");
    signout_screen_group->add(authorized_label, "authorized_label");

    auto resume_button = tgui::Button::create("Продолжить");
    resume_button->setTextSize(30);
    resume_button->onPress([&]() { change_screen(screen_type::MULTIPLAYER); });
    signout_screen_group->add(resume_button, "resume_button");

    auto signout_button = tgui::Button::create("Выйти из аккаунта");
    signout_button->setTextSize(30);
    signout_button->onPress([&]() {
        std::fstream f("../client/configs/client_config.txt");
        client::instance().disconnect();
        client::instance().set_is_authorized(false);
        // client::instance().connect(client::instance().get_server_ip(),
        // client::instance().get_server_port());
        change_screen(screen_type::LOGIN_OR_AUTHORIZATION);
    });
    signout_screen_group->add(signout_button, "return_button");

    auto return_button = tgui::Button::create("Назад");
    return_button->setRenderer(black_theme.getRenderer("Button"));
    return_button->setTextSize(30);
    return_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU); });
    signout_screen_group->add(return_button, "return_button");

    std::vector<tgui::Widget::Ptr> widgets = {authorized_label, resume_button, signout_button, return_button};
    place_widgets(widgets);

    m_gui.add(signout_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::SIGNOUT));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::SIGNOUT))->setVisible(false);
}

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
    login_box->setInputValidator("[a-zA-Z0-9]{0,10}");
    login_box->setTextSize(30);
    login_box->setDefaultText("Логин");
    login_screen_group->add(login_box, "login_box");

    auto password_box = tgui::EditBox::create();
    password_box->setInputValidator("[a-zA-Z0-9]{0,20}");
    password_box->setTextSize(30);
    password_box->setDefaultText("Пароль");
    password_box->setPasswordCharacter('*');
    login_screen_group->add(password_box, "password_box");

    auto make_visible_button = tgui::Button::create();
    make_visible_button->setSize("5.625%", "10%");
    make_visible_button->setPosition("64.375%", "22.4%");
    make_visible_button->getRenderer()->setTexture("../client/resources/menu/visible.png");
    make_visible_button->getRenderer()->setTextureHover("../client/resources/menu/visible.png");
    make_visible_button->getRenderer()->setTextureFocused("../client/resources/menu/visible.png");
    make_visible_button->onPress([&]() {
        auto button = screen_handler::instance()
                          .get_gui()
                          .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                          ->cast<tgui::Group>()
                          ->get("password_box")
                          ->cast<tgui::EditBox>();
        if (button->getPasswordCharacter() == '*') {
            button->setPasswordCharacter('\0');

            screen_handler::instance()
                .get_gui()
                .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                ->cast<tgui::Group>()
                ->get("visibility_button")
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTexture("../client/resources/menu/invisible.png");
            screen_handler::instance()
                .get_gui()
                .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                ->cast<tgui::Group>()
                ->get("visibility_button")
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTextureHover("../client/resources/menu/invisible.png");
            screen_handler::instance()
                .get_gui()
                .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                ->cast<tgui::Group>()
                ->get("visibility_button")
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTextureFocused("../client/resources/menu/invisible.png");
        } else {
            button->setPasswordCharacter('*');

            screen_handler::instance()
                .get_gui()
                .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                ->cast<tgui::Group>()
                ->get("visibility_button")
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTexture("../client/resources/menu/visible.png");
            screen_handler::instance()
                .get_gui()
                .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                ->cast<tgui::Group>()
                ->get("visibility_button")
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTextureHover("../client/resources/menu/visible.png");
            screen_handler::instance()
                .get_gui()
                .get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
                ->cast<tgui::Group>()
                ->get("visibility_button")
                ->cast<tgui::Button>()
                ->getRenderer()
                ->setTextureFocused("../client/resources/menu/visible.png");
        }
        button->setText(button->getText());
    });
    login_screen_group->add(make_visible_button, "visibility_button");

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
    login_screen_group->add(signup_button, "login_button");

    auto old_acc_button = tgui::Button::create("Войти с сохраненным аккаунтом");
    old_acc_button->setTextSize(30);
    old_acc_button->onPress([&]() {
        client::instance().clear_messages();
        if (!client::instance().is_connected()) {
            if (client::instance().connect(client::instance().get_server_ip(),
                                           client::instance().get_server_port())) {
                client::instance().login_or_authorize(true);
            }
        } else {
            client::instance().login_or_authorize(true);
        }
    });
    login_screen_group->add(old_acc_button, "old_button");

    auto return_button = tgui::Button::create("Назад");
    return_button->setTextSize(30);
    return_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::START_SCREEN); });
    login_screen_group->add(return_button, "return_button");

    std::vector<tgui::Widget::Ptr> widgets = {login_box, password_box, send_button, signup_button,
                                              return_button, old_acc_button};
    place_widgets(widgets);

    m_gui.add(login_screen_group,
              screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION))
        ->setVisible(false);
}
}  // namespace war_of_ages::client
