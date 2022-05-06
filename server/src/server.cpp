#include "../include/server.h"
#include <cassert>
#include "../include/game_handler.h"
#include "../include/random_matchmaker.h"
#include "../include/room_matchmaker.h"

#define ensure_status(status, valid_status, is_true) \
    if (((status) == (valid_status)) != (is_true))   \
    break

namespace war_of_ages {

server &server::instance() {
    static server inst;
    return inst;
}

void server::set_port(std::uint16_t port) {
    m_port = port;
}

server::server() : server_interface<messages_type>(m_port) {
}

void server::send_message(const std::string &handle, const message<messages_type> &msg) {
    server_interface::send_message(m_connection_by_id.at(m_id_by_handle.at(handle)), msg);
}

user_status server::get_user_status(std::uint32_t user_id) const {
    std::unique_lock l(m_mutex_status);
    return m_status_by_id.at(user_id);
}

void server::set_user_status(std::uint32_t user_id, user_status new_status) {
    std::unique_lock l(m_mutex_status);
    m_status_by_id[user_id] = new_status;
}

bool server::on_client_connect(std::shared_ptr<connection<messages_type>> client) {
    m_connection_by_id.insert({client->get_id(), client});
    std::unique_lock l(m_mutex_status);
    m_status_by_id.insert({client->get_id(), user_status::MENU});
    return true;
}

void server::on_client_disconnect(std::shared_ptr<connection<messages_type>> client) {
    std::uint32_t id = client->get_id();
    m_connection_by_id.erase(id);
    auto handle_it = m_handle_by_id.find(id);
    if (handle_it != m_handle_by_id.end()) {
        m_id_by_handle.erase(handle_it->second);
        m_handle_by_id.erase(handle_it);
    }
    std::unique_lock l(m_mutex_status);
    m_status_by_id.erase(id);
    // TODO: tournament & matches logic
}

void server::on_message(std::shared_ptr<connection<messages_type>> client, message<messages_type> msg) {
    std::uint32_t valid_size = valid_body_size.at(msg.header.id);
    if (msg.header.size != valid_size && valid_size != -1) {
        client->disconnect();
        return;
    }

    std::uint32_t uid = client->get_id();
    user_status status = get_user_status(uid);
    if (status == user_status::AUTHORIZATION) {
        if (msg.header.id != messages_type::AUTH_LOGIN) {
            return;
        }
        // TODO: handle login attempt
        return;
    }
    assert(m_handle_by_id.find(uid) != m_handle_by_id.end());
    const std::string &handle = m_handle_by_id.at(uid);

    switch (msg.header.id) {
        case messages_type::AUTH_LOGOUT: {
            // TODO
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
            if (random_matchmaker::instance().add_user(handle)) {
                set_user_status(uid, user_status::RANDOMGAME);
            }
        } break;
        case messages_type::RANDOMGAME_LEAVE: {
            ensure_status(status, user_status::RANDOMGAME, true);
            if (random_matchmaker::instance().remove_user(handle)) {
                set_user_status(uid, user_status::MENU);
            }
        } break;
        case messages_type::ROOM_JOIN: {
            ensure_status(status, user_status::MENU, true);
            std::string room_id;
            msg.extract_container(room_id);
            if (room_matchmaker::instance().add_user_to_room(handle, room_id)) {
                set_user_status(uid, user_status::ROOM);
            }
        } break;
        case messages_type::ROOM_LEAVE: {
            ensure_status(status, user_status::ROOM, true);
            if (room_matchmaker::instance().remove_user_from_room(handle)) {
                set_user_status(uid, user_status::MENU);
            }
        } break;
        case messages_type::ROOM_CHANGE_STATUS: {
            ensure_status(status, user_status::ROOM, true);
            room_matchmaker::instance().switch_readiness(handle);
        } break;
        // TODO: tournament messages (by Ilya)
        case messages_type::SERVER_PING: {
        } break;
        default:
            break;
    }
}

}  // namespace war_of_ages