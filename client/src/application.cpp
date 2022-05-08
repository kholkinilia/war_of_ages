#include "../include/application.h"
#include "../include/screen_handler.h"
#include "../include/sfml_printer.h"

namespace war_of_ages {

void application::init() {
    sfml_printer::instance().init();
    screen_handler::instance().init(sfml_printer::instance().get_window());
}

void application::run() {
    while (sfml_printer::instance().get_window().isOpen()) {
        screen_handler::instance().update();
        sfml_printer::instance().update();

        update_screens();

        screen_handler::instance().draw();
        sfml_printer::instance().draw();
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
            single_player_handler::instance().update_game();
            sfml_printer::instance().print_game(single_player_handler::instance().get_snapshot());
        } break;
        default:
            break;
    }
}

}  // namespace war_of_ages
