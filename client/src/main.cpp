#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Loading/ImageLoader.hpp>
#include <chrono>
#include <memory>
#include <random>
#include "../include/client.h"
#include "../include/end_game_screen.h"
#include "../include/game_object_size_constants.h"
#include "../include/screens.h"
#include "../include/ui_functions.h"

namespace war_of_ages {

war_of_ages::client_state current_state;

}  // namespace war_of_ages

int main() {
    sf::RenderWindow window{sf::VideoMode::getFullscreenModes()[0], "War of Ages", sf::Style::Fullscreen};
    tgui::Gui gui(window);
    tgui::Theme::setDefault("../client/resources/tgui_themes/Black.txt");
    window.setVerticalSyncEnabled(true);
    tgui::Vector2u imageSize;
    std::unique_ptr<std::uint8_t[]> imagePixels = tgui::ImageLoader::loadFromFile("../client/resources/pictures/cursor.png", imageSize);
    if (imagePixels)
        tgui::Cursor::setStyle(tgui::Cursor::Type::Arrow, imagePixels.get(), imageSize, {0,0});

    sf::View view = window.getDefaultView();

    war_of_ages::current_state.create_audio_player();

    war_of_ages::screens_init(view, gui);

    gui.get(war_of_ages::current_state.get_cur_screen_id())->setVisible(true);

    war_of_ages::setup_fps(gui);

    float last_added_time = -100;
    float next_creation = rand() % 4 + 1;

    while (window.isOpen()) {
        war_of_ages::handle_window_events(gui, &window, &view);

        if (war_of_ages::current_state.get_cur_screen() != war_of_ages::screen::GAME_SCREEN) {
            view.setCenter(war_of_ages::BACKGROUND_WIDTH / 2, war_of_ages::BACKGROUND_HEIGHT / 2);
        }

        war_of_ages::check_game_end(gui);

        window.clear(sf::Color(50, 50, 50));

        war_of_ages::update_screens(gui, war_of_ages::current_state, &window);

        gui.draw();

        float cur_time = 1.f * clock() / CLOCKS_PER_SEC;
        if (war_of_ages::current_state.get_cur_screen() == war_of_ages::screen::GAME_SCREEN &&
            cur_time - last_added_time >= next_creation) {
            last_added_time = cur_time;
            next_creation = rand() % 4 + 1;
            std::vector<std::unique_ptr<war_of_ages::game_command>> v;
            v.push_back(std::make_unique<war_of_ages::buy_unit_command>(rand() % 3));
            war_of_ages::current_state.get_cur_game_state()->update({}, v, cur_time);
        }

        window.display();
        window.setView(view);
    }
}