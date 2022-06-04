#include "../include/server.h"
#include <cassert>
#include "../include/chat_handler.h"
#include "../include/database_handler.h"
#include "../include/game_handler.h"
#include "../include/random_matchmaker.h"
#include "../include/room_matchmaker.h"
#include "../include/tournament_handler.h"
#include "network.h"

#define ensure_status(status, valid_status, is_true) \
    if (((status) == (valid_status)) != (is_true))   \
    break

namespace war_of_ages::server {

server &server::instance() {
    static server inst;
    return inst;
}

void server::set_port(std::uint16_t port) noexcept {
    m_port = port;
}

server::server() : server_interface<messages_type>(m_port) {
}

void server::send_message(const std::string &handle, const message<messages_type> &msg) {
    std::unique_lock l(m_mutex);
    if (m_id_by_handle.count(handle) == 0) {
        return;
    }
    auto connection = m_connection_by_id.at(m_id_by_handle.at(handle));
    l.unlock();
    server_interface::send_message(connection, msg);
}

server::user_status server::get_user_status(std::uint32_t uid) const {
    std::unique_lock l(m_mutex);
    return m_status_by_id.at(uid);
}

void server::set_user_status(const std::string &handle, user_status new_status) {
    std::unique_lock l(m_mutex);
    if (m_id_by_handle.count(handle) == 0) {
        return;
    }
    m_status_by_id.at(m_id_by_handle.at(handle)) = new_status;
}

void server::on_client_validated(std::shared_ptr<connection<messages_type>> client) {
    std::unique_lock l(m_mutex);
    m_connection_by_id.insert({client->get_id(), client});
    m_status_by_id.insert({client->get_id(), user_status::AUTHORIZATION});
}

bool server::on_client_connect(std::shared_ptr<connection<messages_type>> client) {
    return true;
}

void server::on_client_disconnect(std::shared_ptr<connection<messages_type>> client) {
    // Create new thread to avoid deadlocks
    std::thread([this, client = std::move(client)]() {
        std::uint32_t uid = client->get_id();
        std::unique_lock l(m_mutex);
        if (m_connection_by_id.count(uid) == 0) {
            // Already cleared
            return;
        }
        auto status = m_status_by_id.at(uid);
        m_connection_by_id.erase(uid);
        m_status_by_id.erase(uid);
        if (status != user_status::AUTHORIZATION) {
            auto handle_it = m_handle_by_id.find(uid);
            assert(handle_it != m_handle_by_id.end());
            std::string handle = handle_it->second;
            m_id_by_handle.erase(handle);
            m_handle_by_id.erase(handle_it);
            l.unlock();

            game_handler::instance().user_disconnected(handle);
            tournament_handler::instance().leave(handle);
            room_matchmaker::instance().remove_user_from_room(handle);
        }
        m_finished_on_disconnect.notify_one();
    }).detach();
}

void server::on_message(std::shared_ptr<connection<messages_type>> client, message<messages_type> msg) {
    //    std::cerr << "Received message: " << msg << std::endl;
    if (std::uint32_t valid_size = valid_body_size.at(msg.header.id);
        msg.header.size != valid_size && valid_size != -1) {
        client->disconnect();
        return;
    }
    std::uint32_t uid = client->get_id();
    user_status status = get_user_status(uid);
    if (status == user_status::AUTHORIZATION) {
        if (msg.header.id != messages_type::AUTH_LOGIN && msg.header.id != messages_type::AUTH_REGISTER) {
            return;
        }
        message<messages_type> msg_response;

        std::string user_password, user_handle;
        msg.extract_container(user_password);
        msg.extract_container(user_handle);

        //---------------------------
        // TODO: handle login/register attempt
        message<messages_type> response;
        if (msg.header.id == messages_type::AUTH_LOGIN) {
            if (!database_handler::get_instance().sign_in(user_handle, user_password)) {
                response.header.id = messages_type::AUTH_LOGIN_FAILED;
            } else {
                response.header.id = messages_type::AUTH_LOGIN_SUCCEEDED;
            }
        } else {
            if (!database_handler::get_instance().registration(user_handle, user_password)) {
                response.header.id = messages_type::AUTH_REGISTER_FAILED;
            } else {
                response.header.id = messages_type::AUTH_REGISTER_SUCCEEDED;
            }
        }
        server_interface::send_message(client, response);
        if (response.header.id == messages_type::AUTH_REGISTER_FAILED ||
            response.header.id == messages_type::AUTH_LOGIN_FAILED) {
            return;
        }
        //---------------------------

        // Success (server logic):

        std::unique_lock l(m_mutex);

        if (auto id_it = m_id_by_handle.find(user_handle); id_it != m_id_by_handle.end()) {
            auto &other_connection = m_connection_by_id.at(id_it->second);
            if (other_connection->is_connected()) {
                msg_response.header.id = messages_type::AUTH_ALREADY_USING;
                l.unlock();
                send_message(user_handle, msg_response);
                return;
            } else {
                on_client_disconnect(other_connection);
            }
        }

        // wait for erasing old info
        m_finished_on_disconnect.wait(
            l, [this, &user_handle]() { return m_id_by_handle.count(user_handle) == 0; });

        m_handle_by_id.insert({uid, user_handle});
        m_id_by_handle.insert({user_handle, uid});
        m_status_by_id[uid] = user_status::MENU;
        return;
    }
    std::unique_lock l(m_mutex);
    assert(m_handle_by_id.find(uid) != m_handle_by_id.end());
    const std::string &handle = m_handle_by_id.at(uid);
    l.unlock();

    switch (msg.header.id) {
        case messages_type::AUTH_LOGOUT: {
        } break;
        case messages_type::GAME_BUY_UNIT: {
            std::uint8_t unit_lvl;
            msg >> unit_lvl;
            game_handler::instance().apply_command(handle, std::make_unique<buy_unit_command>(unit_lvl));
        } break;
        case messages_type::GAME_BUY_CANNON: {
            std::uint8_t cannon_lvl, slot;
            msg >> slot >> cannon_lvl;
            game_handler::instance().apply_command(handle,
                                                   std::make_unique<buy_cannon_command>(cannon_lvl, slot));
        } break;
        case messages_type::GAME_BUY_CANNON_SLOT: {
            game_handler::instance().apply_command(handle, std::make_unique<buy_cannon_slot_command>());
        } break;
        case messages_type::GAME_SELL_CANNON: {
            std::uint8_t slot;
            msg >> slot;
            game_handler::instance().apply_command(handle, std::make_unique<sell_cannon_command>(slot));
        } break;
        case messages_type::GAME_USE_ULT: {
            game_handler::instance().apply_command(handle, std::make_unique<use_ult_command>());
        } break;
        case messages_type::GAME_UPGRADE_AGE: {
            game_handler::instance().apply_command(handle, std::make_unique<upgrade_age_command>());
        } break;
        case messages_type::GAME_GIVE_UP: {
            ensure_status(status, user_status::MENU, false);
            ensure_status(status, user_status::AUTHORIZATION, false);
            game_handler::instance().user_gave_up(handle);
        } break;
        case messages_type::RANDOMGAME_JOIN: {
            ensure_status(status, user_status::MENU, true);
            random_matchmaker::instance().add_user(handle);
        } break;
        case messages_type::RANDOMGAME_LEAVE: {
            ensure_status(status, user_status::RANDOMGAME, true);
            if (random_matchmaker::instance().remove_user(handle)) {
                set_user_status(handle, user_status::MENU);
            }
        } break;
        case messages_type::ROOM_JOIN: {
            ensure_status(status, user_status::MENU, true);
            std::string room_id;
            msg.extract_container(room_id);
            if (room_matchmaker::instance().add_user_to_room(handle, room_id)) {
                set_user_status(handle, user_status::ROOM);
            }
        } break;
        case messages_type::ROOM_LEAVE: {
            ensure_status(status, user_status::ROOM, true);
            if (room_matchmaker::instance().remove_user_from_room(handle)) {
                set_user_status(handle, user_status::MENU);
            }
        } break;
        case messages_type::ROOM_SWITCH_STATUS: {
            ensure_status(status, user_status::ROOM, true);
            room_matchmaker::instance().switch_readiness(handle);
        } break;
        case messages_type::TOURNAMENT_CREATE: {
            ensure_status(status, user_status::MENU, true);
            std::string name;
            msg >> name;
            set_user_status(handle, user_status::TOURNAMENT);
            tournament_handler::instance().create(handle, name);
        } break;
        case messages_type::TOURNAMENT_JOIN: {
            ensure_status(status, user_status::MENU, true);
            std::string key;
            msg >> key;
            set_user_status(handle, user_status::TOURNAMENT);
            tournament_handler::instance().join(handle, key);
        } break;
        case messages_type::TOURNAMENT_LEAVE: {
            ensure_status(status, user_status::TOURNAMENT, true);
            set_user_status(handle, user_status::MENU);
            tournament_handler::instance().leave(handle);
        } break;
        case messages_type::SERVER_PING: {
        } break;
        case messages_type::CHAT_NEW_MESSAGE: {
            std::string chat_id, content;
            msg >> content >> chat_id;
            chat_handler::instance().add_message(chat_id, handle, content);
        }
        case messages_type::STATS_GET: {
            message<messages_type> message;
            message.header.id = messages_type::STATS_RESPONSE;
            auto statistics = database_handler::get_instance().get_statistics(handle);
            for (const auto &game : statistics) {
                message << static_cast<int8_t>(game.result) << static_cast<int16_t>(game.opponent_rating)
                        << game.opponent_handle << static_cast<int16_t>(game.rating);
            }
            message << static_cast<int16_t>(database_handler::get_instance().get_rating(handle));
            message << static_cast<int16_t>(statistics.size());
            send_message(handle, message);
        }
        default:
            break;
    }
}

}  // namespace war_of_ages::server