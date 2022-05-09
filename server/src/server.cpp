#include "../include/server.h"
#include <cassert>
#include "../include/game_handler.h"
#include "../include/random_matchmaker.h"
#include "../include/room_matchmaker.h"
#include "../include/tournament_handler.h"

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
//    std::cout << "SENDING MESSAGE to '" << handle << "': " << msg << std::endl;
    server_interface::send_message(m_connection_by_id.at(m_id_by_handle.at(handle)), msg);
}

user_status server::get_user_status(std::uint32_t user_id) const {
    std::unique_lock l(m_mutex);
    std::cout << "Try: uid == " << user_id << std::endl;
    return m_status_by_id.at(user_id);
}

void server::set_user_status(std::uint32_t user_id, user_status new_status) {
    std::unique_lock l(m_mutex);
    m_status_by_id[user_id] = new_status;
}

bool server::on_client_connect(std::shared_ptr<connection<messages_type>> client) {
    static int cnt = 10000;

    std::unique_lock l(m_mutex);
    m_connection_by_id.insert({cnt, client});
    m_status_by_id.insert({cnt, user_status::AUTHORIZATION});
    std::cout << "INSERETED {" << cnt << ", user_status::AUTHORIZATION}" << std::endl;
    cnt++;
    return true;
}

void server::on_client_disconnect(std::shared_ptr<connection<messages_type>> client) {
    std::uint32_t uid = client->get_id();
    auto status = get_user_status(uid);

    std::unique_lock l(m_mutex);
    m_connection_by_id.erase(uid);
    m_status_by_id.erase(uid);
    if (status != user_status::AUTHORIZATION) {
        auto handle_it = m_handle_by_id.find(uid);
        assert(handle_it != m_handle_by_id.end());
        m_id_by_handle.erase(handle_it->second);
        m_handle_by_id.erase(handle_it);
        l.unlock();

        tournament_handler::instance().leave(handle_it->second);
        // TODO: matches logic
    }
}

void server::on_message(std::shared_ptr<connection<messages_type>> client, message<messages_type> msg) {
    std::cout << "on_message(" << int(msg.header.id) << ", " << msg.header.size << ")" << std::endl;
    if (std::uint32_t valid_size = valid_body_size.at(msg.header.id);
        msg.header.size != valid_size && valid_size != -1) {
        client->disconnect();
        return;
    }
    std::cout << "BREAKPOINT 1" << std::endl;
    std::uint32_t uid = client->get_id();
    std::cout << "BREAKPOINT 2" << std::endl;
    user_status status = get_user_status(uid);
    std::cout << "BREAKPOINT 3" << std::endl;
    if (status == user_status::AUTHORIZATION) {
        if (msg.header.id != messages_type::AUTH_LOGIN) {
            return;
        }
        std::cout << "BREAKPOINT 4" << std::endl;
        std::uint32_t uid = client->get_id();
        std::string user_password, user_handle;
        msg.extract_container(user_password);
        msg.extract_container(user_handle);
        std::cout << "BREAKPOINT 5" << std::endl;

        std::unique_lock l(m_mutex);
        m_handle_by_id.insert({uid, user_handle});
        m_id_by_handle.insert({user_handle, uid});
        m_status_by_id[uid] = user_status::MENU;
        std::cout << "BREAKPOINT 6" << std::endl;
        // TODO: handle login attempt
        return;
    }
    std::cout << "MUST NOT BE HERE!" << std::endl;
    std::unique_lock l(m_mutex);
    assert(m_handle_by_id.find(uid) != m_handle_by_id.end());
    const std::string &handle = m_handle_by_id.at(uid);
    l.unlock();

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
            std::cout << "TRYING TO ENTER TO RANDOMGAME..." << std::endl;
            ensure_status(status, user_status::MENU, true);
            if (random_matchmaker::instance().add_user(handle)) {
                std::cout << "SUCCEEDED" << std::endl;
                set_user_status(uid, user_status::RANDOMGAME);
            } else {
                std::cout << "FAILED" << std::endl;
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
        case messages_type::TOURNAMENT_CREATE: {
            ensure_status(status, user_status::TOURNAMENT, true);
            std::string name;
            msg.extract_container(name);
            tournament_handler::instance().create(handle, name);
        } break;
        case messages_type::TOURNAMENT_JOIN: {
            ensure_status(status, user_status::TOURNAMENT, true);
            std::string key;
            msg.extract_container(key);
            tournament_handler::instance().join(handle, key);
        } break;
        case messages_type::TOURNAMENT_LEAVE: {
            ensure_status(status, user_status::TOURNAMENT, true);
            tournament_handler::instance().leave(handle);
        } break;
        case messages_type::SERVER_PING: {
        } break;
        default:
            break;
    }
}

}  // namespace war_of_ages::server