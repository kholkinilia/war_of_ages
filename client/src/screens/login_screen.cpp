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

int is_login = -1;

void screen_handler::login_choice_screen_init() {
    auto login_screen_group = tgui::Group::create();
    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");

    auto login_button = tgui::Button::create("Войти");
    login_button->setRenderer(black_theme.getRenderer("Button"));
    login_button->setTextSize(30);
    login_button->onPress([&]() {
        screen_handler::instance().change_screen(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION);
        is_login = 1;
    });
    login_screen_group->add(login_button, "login_button");

    auto authorization_button = tgui::Button::create("Зарегистрироваться");
    authorization_button->setRenderer(black_theme.getRenderer("Button"));
    authorization_button->setTextSize(30);
    authorization_button->onPress([&]() {
        screen_handler::instance().change_screen(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION);
        is_login = 0;
    });
    login_screen_group->add(authorization_button, "authorization_button");

    std::vector<tgui::Widget::Ptr> widgets = {login_button, authorization_button};
    place_widgets(widgets);

    auto return_button = tgui::Button::create("В главное меню");
    return_button->setRenderer(black_theme.getRenderer("Button"));
    return_button->setTextSize(30);
    return_button->onPress([&]() {
        screen_handler::instance().change_screen(screen_handler::screen_type::START_SCREEN);
    });
    return_button->setPosition("30%", "86%");
    return_button->setSize("40%", "10%");
    login_screen_group->add(return_button, "return_button");
    m_gui.add(login_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION_CHOICE));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION_CHOICE))->setVisible(false);
}

void screen_handler::login_screen_init() {
    auto login_screen_group = tgui::Group::create();
    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");

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

    auto send_button = tgui::Button::create("Войти");
    send_button->setRenderer(black_theme.getRenderer("Button"));
    send_button->setTextSize(30);
    send_button->onPress([&]() {
        if(is_from_statistics) {
            screen_handler::instance().change_screen(screen_handler::screen_type::STATISTICS);
            is_from_statistics = false;
        } else {
            screen_handler::instance().change_screen(screen_handler::screen_type::MULTIPLAYER);
            application::instance().set_state(application::state::MULTIPLAYER);
        }
    });
    login_screen_group->add(send_button, "send_button");

    auto existing_name = tgui::Label::create();
    existing_name->getRenderer()->setTextSize(35);
    existing_name->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    existing_name->getRenderer()->setTextColor(tgui::Color::White);
    existing_name->setText("Такое имя уже существует");
    existing_name->setVisible(false);
    login_screen_group->add(existing_name, "existing_name_label");

    auto wrong_password = tgui::Label::create();
    wrong_password->getRenderer()->setTextSize(35);
    wrong_password->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    wrong_password->getRenderer()->setTextColor(tgui::Color::White);
    wrong_password->setText("Неверный логин/пароль");
    wrong_password->setVisible(false);
    login_screen_group->add(wrong_password, "wrong_password_label");

    std::vector<tgui::Widget::Ptr> widgets = {login_box, password_box, send_button, wrong_password};
    place_widgets(widgets);

    existing_name->setSize(wrong_password->getSize());
    existing_name->setPosition(wrong_password->getPosition());

    auto return_button = tgui::Button::create("Назад");
    return_button->setRenderer(black_theme.getRenderer("Button"));
    return_button->setTextSize(30);
    return_button->onPress([&]() {
        screen_handler::instance().change_screen(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION_CHOICE);
    });
    return_button->setPosition("30%", "86%");
    return_button->setSize("40%", "10%");
    login_screen_group->add(return_button, "return_button");

    m_gui.add(login_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::LOGIN_OR_AUTHORIZATION))->setVisible(false);
}
}  // namespace war_of_ages::client
