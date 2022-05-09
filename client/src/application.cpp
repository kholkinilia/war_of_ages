#include "../include/application.h"
#include "../include/client.h"
#include "../include/multiplayer_snapshots_handler.h"
#include "../include/screen_handler.h"
#include "../include/sfml_printer.h"
#include "../include/single_player_handler.h"
#include "../include/tournament_handler.h"

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

                        msg >> snapshot_p2.m_training_time_left >> snapshot_p2.m_ult_cooldown >>
                            snapshot_p2.money >> snapshot_p2.exp >> snapshot_p2.age;
                        msg.extract_container(snapshot_p2.units_to_train);
                        msg.extract_container(snapshot_p2.cannons);
                        msg.extract_container(snapshot_p2.bullets);
                        msg.extract_container(snapshot_p2.units);

                        msg >> snapshot_p1.m_training_time_left >> snapshot_p1.m_ult_cooldown >>
                            snapshot_p1.money >> snapshot_p1.exp >> snapshot_p1.age;
                        msg.extract_container(snapshot_p1.units_to_train);
                        msg.extract_container(snapshot_p1.cannons);
                        msg.extract_container(snapshot_p1.bullets);
                        msg.extract_container(snapshot_p1.units);

                        multiplayer_snapshots_handler::instance().set_snapshots({snapshot_p1, snapshot_p2});
                    } break;
                    case messages_type::TOURNAMENT_STATE: {
                        tournament_snapshot snapshot;
                        msg.extract_container(snapshot.match_results);
                        msg.extract_container(snapshot.participants);
                        msg.extract_container(snapshot.name);
                        msg.extract_container(snapshot.key);

                        tournament_handler::instance().set_tournament(snapshot);
                        screen_handler::instance().change_screen(
                            screen_handler::screen_type::TOURNAMENT_MAIN);
                    } break;
                    case messages_type::TOURNAMENT_ADD_PLAYER: {
                        std::string handle;
                        msg.extract_container(handle);
                        tournament_handler::instance().add_participant(handle);
                    } break;
                    case messages_type::TOURNAMENT_REMOVE_PLAYER: {
                        std::string handle;
                        msg.extract_container(handle);
                        tournament_handler::instance().remove_participant(handle);
                    } break;
                    case messages_type::TOURNAMENT_ADD_RESULT: {
                        std::string winner, loser;
                        msg.extract_container(loser);
                        msg.extract_container(winner);
                        tournament_handler::instance().add_result(winner, loser);
                    }
                    default:
                        break;
                }
            }
            if (screen_handler::instance().get_screen_type() == screen_handler::screen_type::GAME_SCREEN) {
                sfml_printer::instance().print_game(
                    multiplayer_snapshots_handler::instance().get_snapshots());
            }
        } break;
        case state::MENU: {
        } break;
        default:
            assert(!"Unreachable code!");
    }
}

}  // namespace war_of_ages::client
