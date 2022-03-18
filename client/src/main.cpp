#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
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
    sf::Vector2f oldPos;
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
                        oldPos =
                            window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
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

                    const sf::Vector2f newPos =
                        window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

                    const sf::Vector2f deltaPos = oldPos - newPos;
                    if (view.getCenter().x + deltaPos.x < war_of_ages::BACKGROUND_WIDTH / 2 ||
                        view.getCenter().x + deltaPos.x >
                            war_of_ages::ROAD_WIDTH - war_of_ages::BACKGROUND_WIDTH / 2)
                        break;
                    view.move(deltaPos.x, 0.0f);
                    printer.update(deltaPos.x);

                    oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    break;
                }
            }
        }

        if (war_of_ages::current_state.get_cur_screen() != war_of_ages::screen::GAME_SCREEN) {
            view.setCenter(war_of_ages::BACKGROUND_WIDTH / 2, war_of_ages::BACKGROUND_HEIGHT / 2);
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