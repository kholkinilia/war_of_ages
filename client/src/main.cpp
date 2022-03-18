#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <chrono>
#include "../include/client.h"
#include "../include/end_game_screen.h"
#include "../include/game_object_size_constants.h"
#include "../include/screen_defines.h"
#include "../include/screens.h"
#include "../include/sprite_printer.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

void screens_init(tgui::Gui &gui) {
    // Widgets of every screen are defined here

    // Ilya's screens

    tournament_screen_init(gui);
    tournament_creation_screen_init(gui);
    tournament_join_screen_init(gui);

    // end Ilya's screens

    // Vakhtang's screens

    start_screen_init(gui);
    multiplayer_screen_init(gui);
    settings_screen_init(gui);

    // end Vakhtang's screens

    // Timur's screens

    opponent_waiting_screen_init(gui);
    game_screen_init(gui);
    end_game_screen_init(gui);
    pause_screen_init(gui);

    // end Timur's screens
}

war_of_ages::client_state current_state;

}  // namespace war_of_ages

int main() {
    sf::RenderWindow window{sf::VideoMode::getFullscreenModes()[0], "War of Ages", sf::Style::Fullscreen};
    tgui::Gui gui(window);

    window.setVerticalSyncEnabled(true);

    war_of_ages::screens_init(gui);

    gui.get(war_of_ages::current_state.get_cur_screen_id())->setVisible(true);

    sf::Texture t;
    t.loadFromFile("../client/resources/pictures/fullHD_kittens.jpg");
    sf::Sprite s;
    s.setTexture(t);

    tgui::Label::Ptr fps_label = tgui::Label::create();
    fps_label->getRenderer()->setTextColor(tgui::Color::Red);
    fps_label->setPosition(0, 0);
    gui.add(fps_label);
    float prev_frames_update_time = 1.f * clock() / CLOCKS_PER_SEC;
    long long frames_counter = 0;
    const int UPDATE_FPS_GAP = 10;

    war_of_ages::sprite_printer printer;
    float prev_x;
    bool moving = false;
    sf::View view = window.getDefaultView();
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            gui.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
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
                    if (!moving ||
                        war_of_ages::current_state.get_cur_screen() != war_of_ages::screen::GAME_SCREEN)
                        break;

                    float delta = prev_x - event.mouseMove.x;
                    if (view.getCenter().x + delta < 1.f * war_of_ages::BACKGROUND_WIDTH / 2) {
                        delta = 1.f * war_of_ages::BACKGROUND_WIDTH / 2 - view.getCenter().x;
                    }
                    if (view.getCenter().x + delta > war_of_ages::ROAD_WIDTH - 1.f * war_of_ages::BACKGROUND_WIDTH / 2) {
                        delta = war_of_ages::ROAD_WIDTH - 1.f * war_of_ages::BACKGROUND_WIDTH / 2 - view.getCenter().x;
                    }
                    view.move(delta, 0.0f);
                    printer.update(delta);

                    prev_x = event.mouseMove.x;
                    break;
                }
                default:
                    break;
            }
        }

        if (war_of_ages::current_state.get_cur_screen() != war_of_ages::screen::GAME_SCREEN) {
            view.setCenter(war_of_ages::BACKGROUND_WIDTH / 2, war_of_ages::BACKGROUND_HEIGHT / 2);
        }

        if (war_of_ages::current_state.get_cur_game_state() != nullptr &&
            war_of_ages::current_state.get_cur_game_state()->get_game_status() !=
                war_of_ages::game_status::PROCESSING) {
            window.clear();
            show_screen(gui, war_of_ages::screen::END_GAME, war_of_ages::screen::GAME_SCREEN);
            gui.get(war_of_ages::screen_id.at(war_of_ages::screen::END_GAME))
                ->cast<tgui::Group>()
                ->get("result_label")
                ->cast<tgui::Label>()
                ->setText(war_of_ages::current_state.get_cur_game_state()->get_game_status() ==
                                  war_of_ages::game_status::P1_WON
                              ? "Поздравляем, Вы победили!"
                              : "Вы проиграли, повезет в следующий раз");
            war_of_ages::current_state.set_cur_game_state(nullptr);
        }

        window.clear();
        window.draw(s);

        war_of_ages::update_screens(gui, war_of_ages::current_state, &window);

        gui.draw();

        if (frames_counter % UPDATE_FPS_GAP == 0) {
            float new_time = 1.f * clock() / CLOCKS_PER_SEC;
            fps_label->setText(std::to_string(UPDATE_FPS_GAP * 1. / (new_time - prev_frames_update_time)));
            prev_frames_update_time = new_time;
        }
        frames_counter++;

        window.display();
        window.setView(view);
    }
}