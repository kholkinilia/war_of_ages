#include "../include/application.h"
#include <TGUI/Widgets/Label.hpp>
#include "../include/client.h"
#include "../include/screen_handler.h"
#include "../include/screens/end_game_screen.h"
#include "../include/sfml_printer.h"

namespace war_of_ages {

void application::init() {
    sfml_printer::instance().init();
    screen_handler::instance().init(sfml_printer::instance().get_window());
}

void application::run() {
    sf::Texture t;
    t.loadFromFile("../client/resources/pictures/fullHD_kittens.jpg");
    sf::Sprite s;
    s.setTexture(t);

    while (sfml_printer::instance().get_window().isOpen()) {
        sfml_printer::instance().handle_window_events();

        if (screen_handler::instance().get_screen_type() != screen_handler::screen_type::GAME_SCREEN) {
            sfml_printer::instance().set_view_center(BACKGROUND_WIDTH / 2, BACKGROUND_HEIGHT / 2);
        }

        war_of_ages::check_game_end(screen_handler::instance().get_gui());

        sfml_printer::instance().get_window().clear();
        sfml_printer::instance().get_window().draw(s);

        screen_handler::instance().update_fps();

        update_screens();

        screen_handler::instance().get_gui().draw();

        sfml_printer::instance().get_window().display();
        sfml_printer::instance().get_window().setView(sfml_printer::instance().get_view());
    }
}

void application::set_state(application::state new_state) {
    m_state = new_state;
}

application::state application::get_state() const noexcept {
    return m_state;
}

application &application::instance() {
    static application app;
    return app;
}

void application::update_screens() {
    switch (screen_handler::instance().get_screen_type()) {
        case screen_handler::screen_type::TOURNAMENT_MAIN: {
            // TODO: uncomment, when tournament logic is implemented
            // state.get_cur_tournament()->update_grid(gui.get(screen_id.at(screen::TOURNAMENT_MAIN))
            //                                            ->cast<tgui::Group>()
            //                                            ->get("tournament_grid")
            //                                            ->cast<tgui::Grid>());
        } break;
        case screen_handler::screen_type::GAME_SCREEN: {
            auto state = current_state.get_cur_game_state();
            auto [p1, p2] = state->snapshot_players();
            state->update(current_state.get_cur_game()->get_actions(0, p2),
                          current_state.get_cur_game()->get_actions(1, p2),
                          static_cast<float>(clock()) / CLOCKS_PER_SEC);
            current_state.get_cur_game()->clear_actions();
            sfml_printer::instance().print_game({p1, p2});
            // TODO: update game and get snapshot
            // sfml_printer::instance().print_game()
        } break;
        default:
            break;
    }
}

}  // namespace war_of_ages
