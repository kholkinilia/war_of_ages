#include "../include/ui_functions.h"
#include <TGUI/Widgets/Group.hpp>
#include "../include/client.h"
#include "../include/menu_background.h"

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width, int height, int space) {
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

void show_screen(tgui::Gui &gui, screen new_screen, screen prev_screen) {
    gui.get(screen_id.at(prev_screen))->setVisible(false);
    gui.get(screen_id.at(new_screen))->setVisible(true);
    if (new_screen != screen::GAME_SCREEN) {
        menu_background_handler::get_instance().start_animation(1.f * clock() / CLOCKS_PER_SEC);
    }
    current_state.set_cur_screen(new_screen);
    if (current_state.get_cur_game_state() == nullptr && new_screen == screen::GAME_SCREEN) {
        current_state.set_cur_game_state(std::make_shared<game_state>(1.f * clock() / CLOCKS_PER_SEC));
    }
    if (new_screen == screen::END_GAME) {
        gui.get(screen_id.at(screen::END_GAME))
            ->cast<tgui::Group>()
            ->get("result_label")
            ->cast<tgui::Label>()
            ->setText(current_state.get_cur_game_state()->get_game_status() == game_status::P1_WON
                          ? "Поздравляем, Вы победили!"
                          : "Вы проиграли, повезет в следующий раз");
        current_state.set_cur_game_state(nullptr);
    }
}

tgui::Layout2d get_layout(int width_percent, int height_percent) {
    return {tgui::String(std::to_string(width_percent) + "%"),
            tgui::String(std::to_string(height_percent) + "%")};
}

void update_fps(std::shared_ptr<tgui::Label> label) {
    static float prev_frames_update_time = 1.f * clock() / CLOCKS_PER_SEC;
    static long long frames_counter = 0;
    const int UPDATE_FPS_GAP = 10;

    if (frames_counter % UPDATE_FPS_GAP == 0) {
        float new_time = 1.f * clock() / CLOCKS_PER_SEC;
        label->setText(std::to_string(UPDATE_FPS_GAP * 1. / (new_time - prev_frames_update_time)));
        prev_frames_update_time = new_time;
    }
    frames_counter++;
}

void setup_fps(tgui::Gui &gui) {
    auto fps_label = tgui::Label::create();
    fps_label->getRenderer()->setTextColor(tgui::Color::Red);
    fps_label->setPosition(0, 0);
    gui.add(fps_label, "fps_label");
}

void handle_window_events(tgui::Gui &gui, sf::RenderWindow *window, sf::View *view) {
    static float prev_x;
    static bool moving = false;

    sf::Event event{};
    while (window->pollEvent(event)) {
        gui.handleEvent(event);
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == 0) {
                    moving = true;
                    prev_x = event.mouseButton.x;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == 0) {
                    moving = false;
                }
                break;
            case sf::Event::MouseMoved: {
                if (!moving || current_state.get_cur_screen() != screen::GAME_SCREEN)
                    break;

                float delta = prev_x - event.mouseMove.x;
                if (view->getCenter().x + delta < 1.f * BACKGROUND_WIDTH / 2) {
                    delta = 1.f * BACKGROUND_WIDTH / 2 - view->getCenter().x;
                }
                if (view->getCenter().x + delta > ROAD_WIDTH - 1.f * BACKGROUND_WIDTH / 2) {
                    delta = ROAD_WIDTH - 1.f * BACKGROUND_WIDTH / 2 - view->getCenter().x;
                }
                view->move(delta, 0.0f);

                prev_x = event.mouseMove.x;
                break;
            }
            default:
                break;
        }
    }
}

}  // namespace war_of_ages
