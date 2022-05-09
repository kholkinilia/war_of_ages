#include "../include/application.h"
#include "../include/client.h"
#include "../include/multiplayer_snapshots_handler.h"
#include "../include/screen_handler.h"
#include "../include/sfml_printer.h"
#include "../include/single_player_handler.h"
#include "../include/tournament_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages::client {

void application::init() {
    sfml_printer::instance().init();
    screen_handler::instance().init(sfml_printer::instance().get_window());
}

void application::run() {
    while (sfml_printer::instance().get_window().isOpen()) {
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

[[nodiscard]] static player_snapshot get_snapshot_from_msg(message<messages_type> &msg) {
    player_snapshot snapshot;
    msg >> snapshot.m_training_time_left >> snapshot.m_ult_cooldown >> snapshot.money >> snapshot.exp >>
        snapshot.age;
    msg.extract_container(snapshot.units_to_train);
    msg.extract_container(snapshot.cannons);
    msg.extract_container(snapshot.bullets);
    msg.extract_container(snapshot.units);
    return snapshot;
}

void application::update_screens() {
    sfml_printer::instance().update();
    screen_handler::instance().update_fps();
    switch (application::instance().get_state()) {
        case state::SINGLE_PLAYER_GAME: {
            single_player_handler::instance().update_game();
            if (single_player_handler::instance().get_game_status() != game_status::PROCESSING) {
                application::instance().set_state(state::MENU);
                screen_handler::instance()
                    .get_gui()
                    .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                    ->get<tgui::Label>("result_label")
                    ->setText(single_player_handler::instance().get_game_status() == game_status::P1_WON
                                  ? "Поздравляем, Вы победили!"
                                  : "Вы проиграли, повезет в следующий раз");
                single_player_handler::instance().finish_game();
                screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
            } else {
                sfml_printer::instance().print_game(single_player_handler::instance().get_snapshot());
            }
        } break;
        case state::MULTIPLAYER: {
            auto messages = client::instance().retrieve_messages();
            for (auto &owned_msg : messages) {
                auto &msg = owned_msg.msg;
                std::cerr << "Received message: " << msg << std::endl;
                switch (msg.header.id) {
                    case messages_type::AUTH_LOGIN: {
                        // TODO
                    } break;
                    case messages_type::GAME_START: {
                        screen_handler::instance().change_screen(screen_handler::screen_type::GAME_SCREEN);
                        // TODO: dodelat
                    } break;
                    case messages_type::GAME_FINISHED: {
                        std::uint8_t we_won = 0;
                        msg >> we_won;
                        screen_handler::instance()
                            .get_gui()
                            .get<tgui::Group>(
                                screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                            ->get<tgui::Label>("result_label")
                            ->setText(we_won ? "Поздравляем, Вы победили!"
                                             : "Вы проиграли, повезет в следующий раз");
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_GIVE_UP: {
                        std::uint8_t we_won = 0;
                        msg >> we_won;
                        screen_handler::instance()
                            .get_gui()
                            .get<tgui::Group>(
                                screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                            ->get<tgui::Label>("result_label")
                            ->setText(we_won ? "Поздравляем, Вы победили!\nСоперник сдался :)"
                                             : "Вы проиграли, повезет в следующий раз\nВы сдались :(");
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_DISCONNECTED: {
                        std::uint8_t we_won;
                        msg >> we_won;
                        assert(we_won == 1);  // else it is very strange
                        screen_handler::instance()
                            .get_gui()
                            .get<tgui::Group>(
                                screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                            ->get<tgui::Label>("result_label")
                            ->setText("Поздравляем, Вы победили!\nСоперник потерял соединение");
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_STATE: {
                        player_snapshot snapshot_p2 = get_snapshot_from_msg(msg);
                        player_snapshot snapshot_p1 = get_snapshot_from_msg(msg);
                        multiplayer_snapshots_handler::instance().set_snapshots({snapshot_p1, snapshot_p2});
                    } break;
                    case messages_type::TOURNAMENT_STATE: {
                        tournament_snapshot snapshot;
                        std::cerr << "1" << std::endl;
                        msg >> snapshot.match_results;
                        std::cerr << "2" << std::endl;
                        msg >> snapshot.participants;
                        std::cerr << "3" << std::endl;
                        msg >> snapshot.name;
                        std::cerr << "4" << std::endl;
                        msg >> snapshot.key;
                        std::cerr << "5" << std::endl;

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
