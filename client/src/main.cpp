#include "../include/end_game_screen.h"
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

    sf::View view = window.getDefaultView();

    war_of_ages::current_state.create_audio_player();

    war_of_ages::screens_init(view, gui);

    gui.get(war_of_ages::current_state.get_cur_screen_id())->setVisible(true);

    war_of_ages::setup_fps(gui);

    while (window.isOpen()) {
        war_of_ages::handle_window_events(gui, &window, &view);

        if (war_of_ages::current_state.get_cur_screen() != war_of_ages::screen::GAME_SCREEN) {
            view.setCenter(war_of_ages::BACKGROUND_WIDTH / 2, war_of_ages::BACKGROUND_HEIGHT / 2);
        }

        war_of_ages::check_game_end(gui);

        window.clear(sf::Color(50, 50, 50));

        war_of_ages::update_screens(gui, war_of_ages::current_state, &window);

        gui.draw();

        window.display();
        window.setView(view);
    }
}