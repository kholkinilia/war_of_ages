#include "../include/screen_handler.h"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "../include/client.h"
#include "../include/screens/end_game_screen.h"
#include "../include/screens/game_screen.h"
#include "../include/screens/multiplayer_screen.h"
#include "../include/screens/settings_screen.h"
#include "../include/screens/start_screen.h"
#include "../include/screens/tournament_screens.h"
#include "../include/screens/wait_screen.h"
#include "../include/sfml_printer.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

void screen_handler::set_window(sf::RenderWindow &window) {
    m_gui.setWindow(window);
}

void screen_handler::change_screen(screen_handler::screen_type new_screen) {
    screen_type prev_screen = m_screen_type;
    m_gui.get(screen_id.at(prev_screen))->setVisible(false);
    m_gui.get(screen_id.at(new_screen))->setVisible(true);
    m_screen_type = new_screen;

    // Settings screen_type does not contain resume_button
    if (new_screen == screen_type::SETTINGS) {
        switch (prev_screen) {
            case screen_type::GAME_SCREEN:
                m_gui.get<tgui::Group>(screen_id.at(screen_type::SETTINGS))
                    ->get("resume_button")
                    ->setVisible(true);
                break;
            case screen_type::START_SCREEN:
                m_gui.get<tgui::Group>(screen_id.at(screen_type::SETTINGS))
                    ->get("resume_button")
                    ->setVisible(false);
                break;
            default:
                break;
        }
    }
    if (new_screen == screen_type::END_GAME) {
        m_gui.get(screen_id.at(screen_type::END_GAME))
            ->cast<tgui::Group>()
            ->get("result_label")
            ->cast<tgui::Label>()
            ->setText(current_state.get_cur_game_state()->get_game_status() == game_status::P1_WON
                          ? "Поздравляем, Вы победили!"
                          : "Вы проиграли, повезет в следующий раз");
        current_state.reset_game();
    }
}

void screen_handler::init(sf::RenderWindow &window) {
    m_gui.setWindow(window);

    tournament_screen_init(m_gui);
    tournament_creation_screen_init(m_gui);
    tournament_join_screen_init(m_gui);
    start_screen_init(m_gui);
    multiplayer_screen_init(m_gui);
    settings_screen_init(sfml_printer::instance().get_view(), m_gui);
    opponent_waiting_screen_init(m_gui);
    game_screen_init(sfml_printer::instance().get_view(), m_gui);
    end_game_screen_init(m_gui);

    m_gui.get(screen_id.at(m_screen_type))->setVisible(true);

    auto fps_label = tgui::Label::create();
    fps_label->getRenderer()->setTextColor(tgui::Color::Red);
    fps_label->setPosition(0, 0);
    m_gui.add(fps_label, "fps_label");
}

screen_handler::screen_type screen_handler::get_screen_type() const noexcept {
    return m_screen_type;
}

tgui::Gui &screen_handler::get_gui() noexcept {
    return m_gui;
}

screen_handler &screen_handler::instance() {
    static screen_handler handler;
    return handler;
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
}  // namespace war_of_ages
