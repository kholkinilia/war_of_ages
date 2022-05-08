#include "../include/application.h"
#include "../include/client.h"
#include "../include/screen_handler.h"
#include "../include/sfml_printer.h"
#include "../include/single_player_handler.h"
#include "../include/multiplayer_snapshots_handler.h"

namespace war_of_ages::client {

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
    switch (application::instance().get_state()) {
        case state::SINGLE_PLAYER_GAME: {
            single_player_handler::instance().update_game();
            sfml_printer::instance().print_game(single_player_handler::instance().get_snapshot());
        } break;
        case state::MULTIPLAYER: {
            auto messages = client::instance().retrieve_messages();
            for (auto &owned_msg : messages) {
                auto &msg = owned_msg.msg;
                switch (msg.header.id) {
                    case messages_type::AUTH_LOGIN:
                        // TODO
                        break;
                    case messages_type::GAME_START:
                        screen_handler::instance().change_screen(screen_handler::screen_type::GAME_SCREEN);
                        // TODO: dodelat
                        break;
                    case messages_type::GAME_FINISHED:
                        // TODO: dodelat
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                        break;
                    case messages_type::GAME_GIVE_UP: {
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_STATE: {
                        player_snapshot snapshot_p1, snapshot_p2;
                        /*
                        std::deque<unit> units;
                        std::vector<bullet> bullets;
                        std::vector<cannon> cannons;
                        std::deque<unit> units_to_train;
                        age_type age;
                        int exp;
                        int money;
                        float m_ult_cooldown;
                        float m_training_time_left;
                         */

                        msg >> snapshot_p2.m_training_time_left >> snapshot_p2.m_ult_cooldown >> snapshot_p2.money >> snapshot_p2.exp;
                        msg.extract_container(snapshot_p2.units_to_train);
                        msg.extract_container(snapshot_p2.cannons);
                        msg.extract_container(snapshot_p2.bullets);
                        msg.extract_container(snapshot_p2.units);

                        msg >> snapshot_p1.m_training_time_left >> snapshot_p1.m_ult_cooldown >> snapshot_p1.money >> snapshot_p1.exp;
                        msg.extract_container(snapshot_p1.units_to_train);
                        msg.extract_container(snapshot_p1.cannons);
                        msg.extract_container(snapshot_p1.bullets);
                        msg.extract_container(snapshot_p1.units);

                        multiplayer_snapshots_handler::instance().set_snapshots({snapshot_p1, snapshot_p2});
                    } break;
                    default:
                        break;
                }
            }

//                        switch (screen_handler::instance().get_screen_type()) {
//                            case screen_handler::screen_type::TOURNAMENT_MAIN: {
//                                // TODO: uncomment, when tournament logic is implemented
//                                //
//                                state.get_cur_tournament()->update_grid(gui.get(screen_id.at(screen::TOURNAMENT_MAIN))
//                                //                                            ->cast<tgui::Group>()
//                                //                                            ->get("tournament_grid")
//                                //                                            ->cast<tgui::Grid>());
//                            } break;
//                            case screen_handler::screen_type::GAME_SCREEN: {
//                            } break;
//                            default:
//                                break;
//                        }
        } break;
        case state::WAITING_FOR_SERVER: {
        } break;
        case state::MENU: {
        } break;
        default:
            assert(!"Unreachable code!");
    }
}

}  // namespace war_of_ages::client
