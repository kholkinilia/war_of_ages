#include "../include/screen_handler.h"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "../include/application.h"
#include "../include/sfml_printer.h"
#include "../include/single_player_handler.h"

namespace war_of_ages::client {

static tgui::Layout2d get_layout(int width_percent, int height_percent) {
    return {tgui::String(std::to_string(width_percent) + "%"),
            tgui::String(std::to_string(height_percent) + "%")};
}

// FIXME: remove this & think about it in other place
void screen_handler::check_game_end() {
    // FIXME: call not only singleplayer
    if (application::instance().get_state() == application::state::SINGLE_PLAYER_GAME &&
        single_player_handler::instance().get_game_status() != game_status::PROCESSING) {
        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
    }
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

    int length = static_cast<int>(widgets.size()) * (height + space) - space;
    int cur_y_pos = (100 - length) / 2;
    int x_pos = (100 - width) / 2;
    for (auto &w : widgets) {
        w->setSize(get_layout(width, height));
        w->setPosition(get_layout(x_pos, cur_y_pos));
        cur_y_pos += height + space;
    }
}

void screen_handler::change_screen(screen_handler::screen_type new_screen) {
    if (new_screen == screen_type::GAME_SCREEN) {
        m_gui.get("background_group")->setVisible(false);
    }
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
        // FIXME: call not only singleplayer (if we make END_GAME screen either for singleplayer&multiplayer)
        m_gui.get(screen_id.at(screen_type::END_GAME))
            ->cast<tgui::Group>()
            ->get("result_label")
            ->cast<tgui::Label>()
            ->setText(single_player_handler::instance().get_game_status() == game_status::P1_WON
                          ? "Поздравляем, Вы победили!"
                          : "Вы проиграли, повезет в следующий раз");
        single_player_handler::instance().finish_game();
    }
}

void screen_handler::init(sf::RenderWindow &window) {
    m_gui.setWindow(window);

    background_init();
    tournament_screen_init();
    tournament_creation_screen_init();
    tournament_join_screen_init();
    start_screen_init();
    multiplayer_screen_init();
    settings_screen_init();
    opponent_waiting_screen_init();
    game_screen_init();
    end_game_screen_init();
    wait_for_server_screen_init();

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

void screen_handler::draw() {
    m_gui.draw();
}

}  // namespace war_of_ages::client
