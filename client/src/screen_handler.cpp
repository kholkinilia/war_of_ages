#include "../include/screen_handler.h"
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "../include/application.h"
#include "../include/background_handler.h"
#include "../include/chat_handler.h"
#include "../include/sfml_printer.h"
#include "../include/single_player_handler.h"

namespace war_of_ages::client {

screen_handler &screen_handler::instance() {
    static screen_handler handler;
    return handler;
}

void screen_handler::init(sf::RenderWindow &window) {
    m_gui.setWindow(window);
    tgui::Theme::setDefault("../client/resources/tgui_themes/Black.txt");
    m_theme_buttons.load("../client/resources/tgui_themes/theme.txt");

    background_handler::init();
    tournament_screen_init();
    tournament_creation_screen_init();
    tournament_join_screen_init();
    room_screen_init();
    start_screen_init();
    multiplayer_screen_init();
    settings_screen_init();
    opponent_waiting_screen_init();
    game_screen_init();
    end_game_screen_init();
    wait_for_server_screen_init();
    statistics_screen_init();
    login_screen_init();
    unauthorized_screen_init();

    //    static chat_handler chat;
    //    chat.show();
    //    chat.add_message("Ilya", "Привет");
    //    chat.add_message("Timur", "Привет");
    //    chat.add_message("Mark", "Привет");
    //    chat.add_message("Alex", "Привет");
    //    chat.add_message("Petr", "Привет");
    //    chat.add_message("Sergey", "Привет");
    //    chat.add_message("Yurij", "Привет");
    //    chat.add_message("Egor", "Привет");

    m_gui.get(screen_id.at(get_screen_type()))->setVisible(true);

    auto fps_label = tgui::Label::create();
    fps_label->getRenderer()->setTextColor(tgui::Color::Red);
    fps_label->setPosition(0, 0);
    m_gui.add(fps_label, "fps_label");
}

void screen_handler::change_screen(screen_handler::screen_type new_screen) {
    screen_type prev_screen = get_screen_type();

    if (new_screen == screen_type::PREVIOUS) {
        m_screen_stack.pop();
        assert(!m_screen_stack.empty());
        new_screen = m_screen_stack.top();
        m_screen_stack.pop();
    } else if (new_screen == screen_type::PREVIOUS_MENU) {
        const static std::vector<screen_type> non_menu_screens{
            screen_type::GAME_SCREEN, screen_type::WAITING_FOR_SERVER, screen_type::SETTINGS,
            screen_type::WAIT_OPPONENT, screen_type::END_GAME};
        m_screen_stack.pop();
        while (!m_screen_stack.empty() &&
               std::count(non_menu_screens.begin(), non_menu_screens.end(), m_screen_stack.top()) != 0) {
            m_screen_stack.pop();
        }
        new_screen = m_screen_stack.top();
        m_screen_stack.pop();
    }

    if (new_screen != screen_type::GAME_SCREEN && prev_screen != screen_type::GAME_SCREEN) {
        background_handler::instance().start_animation();
    }

    if (new_screen == screen_type::GAME_SCREEN) {
        m_gui.get("background_group")->setVisible(false);
    } else if (prev_screen == screen_type::GAME_SCREEN) {
        m_gui.get("background_group")->setVisible(true);
    }

    m_gui.get(screen_id.at(prev_screen))->setVisible(false);
    m_gui.get(screen_id.at(new_screen))->setVisible(true);

    if (new_screen == screen_type::START_SCREEN) {
        while (!m_screen_stack.empty()) {  // wtf no clear function ?
            m_screen_stack.pop();
        }
    }
    m_screen_stack.push(new_screen);

    switch (new_screen) {
        case screen_type::GAME_SCREEN: {
            if (prev_screen == screen_type::START_SCREEN) {
                application::instance().set_state(application::state::SINGLE_PLAYER_GAME);
            }
        } break;
        case screen_type::MULTIPLAYER: {
            application::instance().set_state(application::state::MULTIPLAYER);
        } break;
        case screen_type::START_SCREEN: {
            application::instance().set_state(application::state::MENU);
        } break;
        default:
            break;
    }

    // Settings screen_type does not contain resume_button
    if (new_screen == screen_type::SETTINGS) {
        switch (prev_screen) {
            case screen_type::GAME_SCREEN: {
                m_gui.get<tgui::Group>(screen_id.at(screen_type::SETTINGS))
                    ->get("resume_button")
                    ->setVisible(true);
                m_gui.get<tgui::Group>(screen_id.at(screen_type::SETTINGS))
                    ->get<tgui::Button>("return_button")
                    ->setText("Сдаться");
            } break;
            case screen_type::START_SCREEN: {
                m_gui.get<tgui::Group>(screen_id.at(screen_type::SETTINGS))
                    ->get("resume_button")
                    ->setVisible(false);
                m_gui.get<tgui::Group>(screen_id.at(screen_type::SETTINGS))
                    ->get<tgui::Button>("return_button")
                    ->setText("В главное меню");
            } break;
            default:
                break;
        }
    }

    if (new_screen == screen_type::LOGIN_OR_AUTHORIZATION) {
        m_gui.get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
            ->cast<tgui::Group>()
            ->get("existing_name_label")
            ->cast<tgui::Label>()
            ->setVisible(false);
        m_gui.get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
            ->cast<tgui::Group>()
            ->get("wrong_password_label")
            ->cast<tgui::Label>()
            ->setVisible(false);
        m_gui.get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
            ->cast<tgui::Group>()
            ->get("login_box")
            ->cast<tgui::EditBox>()
            ->setText("");
        m_gui.get(screen_id.at(screen_type::LOGIN_OR_AUTHORIZATION))
            ->cast<tgui::Group>()
            ->get("password_box")
            ->cast<tgui::EditBox>()
            ->setText("");
    }
}

void screen_handler::draw() {
    m_gui.draw();
}

void screen_handler::update_fps() {
    static float prev_frames_update_time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    static long long frames_counter = 0;
    const int UPDATE_FPS_GAP = 10;

    auto label = m_gui.get("fps_label")->cast<tgui::Label>();

    if (frames_counter % UPDATE_FPS_GAP == 0) {
        float new_time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
        label->setText(std::to_string(UPDATE_FPS_GAP * 1. / (new_time - prev_frames_update_time)));
        prev_frames_update_time = new_time;
    }
    frames_counter++;
}

void screen_handler::place_widgets(std::vector<tgui::Widget::Ptr> &widgets,
                                   int width,
                                   int height,
                                   int space) {
    /// Places widgets in the center of a screen in the order specified by their order in widgets vector.
    ///
    /// Width: width of each Widget in percent of screen width. (default = 40)
    /// Height: height of each Widget in percent of screen height. (default = 10)
    /// Space: a gap between each pair of adjacent Widgets in percent of screen height. (default = 5)
    ///
    /// Normally you don't want to specify width, height or space parameters, because it will lead to
    /// differences between screens.

    static auto get_layout = [](int width_percent, int height_percent) -> tgui::Layout2d {
        return {tgui::String(std::to_string(width_percent) + "%"),
                tgui::String(std::to_string(height_percent) + "%")};
    };

    int length = static_cast<int>(widgets.size()) * (height + space) - space;
    int cur_y_pos = (100 - length) / 2;
    int x_pos = (100 - width) / 2;
    for (auto &w : widgets) {
        w->setSize(get_layout(width, height));
        w->setPosition(get_layout(x_pos, cur_y_pos));
        cur_y_pos += height + space;
    }
}

screen_handler::screen_type screen_handler::get_screen_type() const noexcept {
    assert(!m_screen_stack.empty());
    return m_screen_stack.top();
}

tgui::Gui &screen_handler::get_gui() noexcept {
    return m_gui;
}

tgui::Theme &screen_handler::get_theme_buttons() noexcept {
    return m_theme_buttons;
}

screen_handler::screen_handler() {
    m_screen_stack.push(screen_type::START_SCREEN);
}

}  // namespace war_of_ages::client
