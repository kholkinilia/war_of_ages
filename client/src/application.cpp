#include "../include/application.h"
#include "../include/background_handler.h"
#include "../include/client.h"
#include "../include/multiplayer_snapshots_handler.h"
#include "../include/room_handler.h"
#include "../include/screen_handler.h"
#include "../include/sfml_printer.h"
#include "../include/single_player_handler.h"
#include "../include/sound_player.h"
#include "../include/sprite_supplier.h"
#include "../include/tournament_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>

namespace war_of_ages::client {

void application::init() {
    sprite_supplier::start_reading_Q_table();
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

application::multiplayer_state application::get_multiplayer_state() const noexcept {
    return m_multiplayer_state;
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
    background_handler::instance().print_background(screen_handler::instance()
                                                        .get_gui()
                                                        .get("background_group")
                                                        ->cast<tgui::Group>()
                                                        ->get("background_canvas")
                                                        ->cast<tgui::CanvasSFML>());
    switch (m_state) {
        case state::SINGLE_PLAYER_GAME: {
            if (screen_handler::instance().get_screen_type() != screen_handler::screen_type::GAME_SCREEN) {
                break;
            }
            single_player_handler::instance().update_game();
            if (single_player_handler::instance().get_game_status() != game_status::PROCESSING) {
                set_state(state::MENU);
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
                switch (msg.header.id) {
                    case messages_type::AUTH_LOGIN_FAILED: {
                        if (screen_handler::instance().get_screen_type() ==
                            screen_handler::screen_type::LOGIN_OR_AUTHORIZATION) {
                            screen_handler::instance().change_screen(
                                screen_handler::screen_type::UNAUTHORIZED_SCREEN);
                            screen_handler::instance()
                                .get_gui()
                                .get(screen_handler::screen_id.at(
                                    screen_handler::screen_type::UNAUTHORIZED_SCREEN))
                                ->cast<tgui::Group>()
                                ->get("unauthorized_label")
                                ->cast<tgui::Label>()
                                ->setText("Неверный логин/пароль.");
                        } else {
                            screen_handler::instance().change_screen(
                                screen_handler::screen_type::LOGIN_OR_AUTHORIZATION);
                            set_state(state::MULTIPLAYER);
                        }
                        client::instance().set_is_authorized(false);
                    } break;
                    case messages_type::AUTH_REGISTER_FAILED: {
                        screen_handler::instance().change_screen(
                            screen_handler::screen_type::UNAUTHORIZED_SCREEN);
                        screen_handler::instance()
                            .get_gui()
                            .get(screen_handler::screen_id.at(
                                screen_handler::screen_type::UNAUTHORIZED_SCREEN))
                            ->cast<tgui::Group>()
                            ->get("unauthorized_label")
                            ->cast<tgui::Label>()
                            ->setText("Такое имя уже есть.");
                        client::instance().set_is_authorized(false);
                    } break;
                    case messages_type::AUTH_LOGIN_SUCCEEDED:
                    case messages_type::AUTH_REGISTER_SUCCEEDED: {
                        if (screen_handler::instance().get_screen_type() ==
                            screen_handler::screen_type::LOGIN_OR_AUTHORIZATION) {
                            std::ofstream login_password("../client/configs/client_config.txt");
                            login_password << client::instance().get_handle() << std::endl;
                            login_password << client::instance().get_password();
                            screen_handler::instance().change_screen(
                                screen_handler::screen_type::MULTIPLAYER);
                        } else {
                            screen_handler::instance().change_screen(screen_handler::screen_type::SIGNOUT);
                            screen_handler::instance()
                            .get_gui()
                            .get(screen_handler::screen_id.at(
                                screen_handler::screen_type::SIGNOUT))
                                ->cast<tgui::Group>()
                                ->get("authorized_label")
                                ->cast<tgui::Label>()
                                ->setText("Вы вошли в сеть, ваш хендл: " + client::instance().get_handle());
                            set_state(state::MULTIPLAYER);
                        }
                        client::instance().set_is_authorized(true);
                    } break;
                    case messages_type::AUTH_ALREADY_USING: {
                        screen_handler::instance().change_screen(
                            screen_handler::screen_type::UNAUTHORIZED_SCREEN);
                        screen_handler::instance()
                        .get_gui()
                        .get(screen_handler::screen_id.at(
                            screen_handler::screen_type::UNAUTHORIZED_SCREEN))
                            ->cast<tgui::Group>()
                            ->get("unauthorized_label")
                            ->cast<tgui::Label>()
                            ->setText("Этот пользователь уже онлайн");
                        client::instance().set_is_authorized(false);
                    } break;
                    case messages_type::GAME_START: {
                        std::cerr << "GOT GAME_START" << std::endl;
                        multiplayer_snapshots_handler::instance().reset();

                        std::string enemy_handle;
                        msg.extract_container(enemy_handle);

                        screen_handler::instance()
                            .get_gui()
                            .get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))
                            ->cast<tgui::Group>()
                            ->get("enemy_handle_label")
                            ->cast<tgui::Label>()
                            ->setText(enemy_handle);

                        screen_handler::instance().change_screen(screen_handler::screen_type::GAME_SCREEN);
                        sound_player::instance().change(sound_player::sound_type::LOBBY,
                                                        sound_player::sound_type::BATTLE);
                    } break;
                    case messages_type::GAME_FINISHED: {
                        std::cerr << "GOT GAME_FINISHED" << std::endl;
                        std::uint8_t we_won = 0;
                        msg >> we_won;
                        screen_handler::instance()
                            .get_gui()
                            .get<tgui::Group>(
                                screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                            ->get<tgui::Label>("result_label")
                            ->setText(we_won ? "Поздравляем, Вы победили!"
                                             : "Вы проиграли, повезет в следующий раз");
                        if (m_multiplayer_state == multiplayer_state::ROOM) {
                            room_handler::instance().change_my_status(room_handler::player_status::NOT_READY);
                            room_handler::instance().change_enemy_status(
                                room_handler::player_status::NOT_READY);
                        }
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_GIVE_UP: {
                        std::cerr << "GOT GAME_GIVE_UP" << std::endl;
                        std::uint8_t we_won = 0;
                        msg >> we_won;
                        screen_handler::instance()
                            .get_gui()
                            .get<tgui::Group>(
                                screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                            ->get<tgui::Label>("result_label")
                            ->setText(we_won ? "Поздравляем, Вы победили!\nСоперник сдался :)"
                                             : "Вы проиграли, повезет в следующий раз\nВы сдались :(");
                        if (m_multiplayer_state == multiplayer_state::ROOM) {
                            room_handler::instance().change_my_status(room_handler::player_status::NOT_READY);
                            room_handler::instance().change_enemy_status(
                                room_handler::player_status::NOT_READY);
                        }
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_DISCONNECTED: {
                        std::cerr << "GOT GAME_DISCONNECTED" << std::endl;
                        std::uint8_t we_won;
                        msg >> we_won;
                        assert(we_won == 1);  // else it is very strange
                        screen_handler::instance()
                            .get_gui()
                            .get<tgui::Group>(
                                screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))
                            ->get<tgui::Label>("result_label")
                            ->setText("Поздравляем, Вы победили!\nСоперник потерял соединение");
                        if (m_multiplayer_state == multiplayer_state::ROOM) {
                            room_handler::instance().change_my_status(room_handler::player_status::NOT_READY);
                            room_handler::instance().change_enemy_status(
                                room_handler::player_status::NOT_READY);
                        }
                        screen_handler::instance().change_screen(screen_handler::screen_type::END_GAME);
                    } break;
                    case messages_type::GAME_STATE: {
                        player_snapshot snapshot_p2 = get_snapshot_from_msg(msg);
                        player_snapshot snapshot_p1 = get_snapshot_from_msg(msg);
                        multiplayer_snapshots_handler::instance().set_snapshots({snapshot_p1, snapshot_p2});
                    } break;
                    case messages_type::ROOM_JOIN_RESPONSE: {
                        std::cout << "got ROOM_JOIN_RESPONSE" << std::endl;
                        std::uint8_t success;
                        msg >> success;
                        if (success == 0) {
                            screen_handler::instance()
                                .get_gui()
                                .get<tgui::Button>("room_joining_failed_msg")
                                ->setVisible(true);
                            screen_handler::instance()
                                .get_gui()
                                .get<tgui::Group>(screen_handler::screen_id.at(
                                    screen_handler::instance().get_screen_type()))
                                ->setEnabled(false);
                            break;
                        }
                        room_handler::instance().update_me(room_handler::player_info{
                            client::instance().get_handle(), room_handler::player_status::NOT_READY,
                            /*TODO: get rate from client*/ 0});
                        std::string enemy_handle;
                        msg >> enemy_handle;
                        if (!enemy_handle.empty()) {
                            std::uint8_t enemy_status, enemy_rate;
                            msg >> enemy_status >> enemy_rate;
                            room_handler::instance().update_enemy(room_handler::player_info{
                                enemy_handle,
                                (enemy_status == 1 ? room_handler::player_status::READY
                                                   : room_handler::player_status::NOT_READY),
                                enemy_rate});
                        }
                        screen_handler::instance().change_screen(screen_handler::screen_type::ROOM_SCREEN);
                    } break;
                    case messages_type::ROOM_ENEMY_JOINED: {
                        std::cout << "got ROOM_ENEMY_JOINED" << std::endl;
                        std::string enemy_handle;
                        msg >> enemy_handle;
                        std::uint8_t enemy_status, enemy_rate;
                        msg >> enemy_status >> enemy_rate;
                        room_handler::instance().update_enemy(room_handler::player_info{
                            enemy_handle,
                            (enemy_status == 1 ? room_handler::player_status::READY
                                               : room_handler::player_status::NOT_READY),
                            enemy_rate});
                    } break;
                    case messages_type::ROOM_LEAVE_SUCCESS: {
                        std::cout << "GOT ROOM_LEAVE_SUCCESS" << std::endl;
                        room_handler::instance().update_enemy(std::nullopt);
                        screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU);
                    } break;
                    case messages_type::ROOM_ENEMY_LEAVED: {
                        std::cout << "GOT ROOM_ENEMY_LEAVED" << std::endl;
                        room_handler::instance().update_enemy(std::nullopt);
                    } break;
                    case messages_type::ROOM_ENEMY_SWITCHED_STATUS: {
                        std::cout << "GOT ROOM_ENEMY_SWITCHED_STATUS" << std::endl;
                        room_handler::instance().switch_enemy_status();
                    } break;
                    case messages_type::TOURNAMENT_STATE: {
                        tournament_snapshot snapshot;
                        msg >> snapshot.match_results;
                        msg >> snapshot.participants;
                        msg >> snapshot.name;
                        msg >> snapshot.key;

                        tournament_handler::instance().set_tournament(snapshot);
                        tournament_handler::instance().get_chat().show();
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
                    } break;
                    case messages_type::TOURNAMENT_ADD_MATCH: {
                        std::string handle1, handle2;
                        msg >> handle2 >> handle1;
                        tournament_handler::instance().match_participants(handle1, handle2);
                    } break;
                    case messages_type::CHAT_NEW_MESSAGE: {
                        switch (m_multiplayer_state) {
                            case multiplayer_state::TOURNAMENT: {
                                std::string chat_id, handle, content;
                                msg >> content >> handle >> chat_id;
                                tournament_handler::instance().get_chat().add_message(handle, content);
                            } break;
                            default: {
                                break;
                            }
                        }
                    } break;
                    case messages_type::STATS_RESPONSE: {
                        screen_handler::instance().change_screen(screen_handler::screen_type::STATISTICS);
                        auto panel =
                            screen_handler::instance()
                                .get_gui()
                                .get(screen_handler::screen_id.at(screen_handler::screen_type::STATISTICS))
                                ->cast<tgui::Group>()
                                ->get("stats_panel")
                                ->cast<tgui::ScrollablePanel>();
                        int16_t size, my_current_rating, rating, opponent_rating;
                        int8_t result;
                        msg >> size >> my_current_rating;
                        screen_handler::instance()
                            .get_gui()
                            .get(screen_handler::screen_id.at(screen_handler::screen_type::STATISTICS))
                            ->cast<tgui::Group>()
                            ->get("handle_rating_label")
                            ->cast<tgui::Label>()
                            ->setText("Хендл: " + client::instance().get_handle() +
                                      "\t\tРейтинг: " + std::to_string(my_current_rating));
                        std::string opponent_handle;
                        for (int i = 0; i < 16; i++) {
                            if (i >= size) {
                                panel->get("background_" + std::to_string(i))
                                    ->cast<tgui::Panel>()
                                    ->setVisible(false);
                                continue;
                            }

                            msg >> rating >> opponent_handle >> opponent_rating >> result;

                            panel->get("background_" + std::to_string(i))
                                ->cast<tgui::Panel>()
                                ->setVisible(true);

                            panel->get("background_" + std::to_string(i))
                                ->cast<tgui::Panel>()
                                ->getRenderer()
                                ->setBackgroundColor(result == 0 ? tgui::Color::Red : tgui::Color::Green);

                            panel->get("background_" + std::to_string(i))
                                ->cast<tgui::Panel>()
                                ->get("my_handle")
                                ->cast<tgui::Label>()
                                ->setText(client::instance().get_handle());

                            panel->get("background_" + std::to_string(i))
                                ->cast<tgui::Panel>()
                                ->get("my_rating")
                                ->cast<tgui::Label>()
                                ->setText(std::to_string(rating));

                            panel->get("background_" + std::to_string(i))
                                ->cast<tgui::Panel>()
                                ->get("opponent_handle")
                                ->cast<tgui::Label>()
                                ->setText(opponent_handle);

                            panel->get("background_" + std::to_string(i))
                                ->cast<tgui::Panel>()
                                ->get("opponent_rating")
                                ->cast<tgui::Label>()
                                ->setText(std::to_string(opponent_rating));
                        }
                    } break;
                    default:
                        break;
                }
            }
            switch (screen_handler::instance().get_screen_type()) {
                case screen_handler::screen_type::GAME_SCREEN: {
                    if (multiplayer_snapshots_handler::instance().is_snapshot_initialized()) {
                        sfml_printer::instance().print_game(
                            multiplayer_snapshots_handler::instance().get_snapshots());
                    }
                } break;
                case screen_handler::screen_type::TOURNAMENT_MAIN: {
                    tournament_handler::instance().update_grid(
                        screen_handler::instance()
                            .get_gui()
                            .get(screen_handler::screen_id.at(screen_handler::screen_type::TOURNAMENT_MAIN))
                            ->cast<tgui::Group>()
                            ->get("tournament_grid")
                            ->cast<tgui::Grid>());
                } break;
                default:
                    break;
            }
        } break;
        case state::MENU: {
        } break;
        default:
            assert(!"Unreachable code!");
    }
}

void application::set_multiplayer_state(application::multiplayer_state new_state) noexcept {
    m_multiplayer_state = new_state;
}

}  // namespace war_of_ages::client
