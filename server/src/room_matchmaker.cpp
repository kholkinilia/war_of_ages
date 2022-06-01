#include "../include/room_matchmaker.h"
#include <algorithm>
#include <cassert>
#include "../include/game_handler.h"
#include "../include/server.h"

namespace war_of_ages::server {
room_matchmaker &room_matchmaker::instance() {
    static room_matchmaker inst;
    return inst;
}

bool room_matchmaker::add_user_to_room(const std::string &handle, const std::string &room_id) {
    std::unique_lock l(m_mutex_status);
    if (m_users_in_rooms.count(handle)) {
        return false;
    }
    message<messages_type> msg;
    msg.header.id = messages_type::ROOM_JOIN;
    auto &room = m_rooms[room_id];
    if (room.size() == 2) {
        msg << static_cast<std::uint8_t>(0);
        server::instance().send_message(handle, msg);
        return false;
    }
    room.push_back(handle);
    m_users_in_rooms.insert({handle, {room_id, user_in_room::user_status::NOT_READY}});

    if (room.size() == 2) {
        msg << static_cast<std::uint8_t>(0)
            << static_cast<std::uint8_t>(
                   m_users_in_rooms[room[0]].status == user_in_room::user_status::READY ? 1 : 0)
            << room[0];

        message<messages_type> msg2;
        msg2.header.id = messages_type::ROOM_JOIN;
        msg2 << static_cast<std::uint8_t>(0) << static_cast<std::uint8_t>(0) << handle
             << static_cast<std::uint8_t>(1);
        server::instance().send_message(room[0], msg2);
    }
    msg << static_cast<std::uint8_t>(1);
    server::instance().send_message(handle, msg);
    return true;
}

bool room_matchmaker::remove_user_from_room(const std::string &handle) {
    std::unique_lock l(m_mutex_status);
    if (!m_users_in_rooms.count(handle)) {
        return false;
    }
    auto it_user = m_users_in_rooms.find(handle);
    if (it_user->second.status == user_in_room::user_status::FIGHTING) {
        // should finish the battle first
        return false;
    }
    auto &room = m_rooms[it_user->second.room_id];
    room.erase(std::remove(room.begin(), room.end(), handle), room.end());
    m_users_in_rooms.erase(it_user);

    message<messages_type> msg;
    msg.header.id = messages_type::ROOM_LEAVE;
    server::instance().send_message(handle, msg);
    if (!room.empty()) {
        msg << static_cast<std::uint8_t>(1);
        server::instance().send_message(room[0], msg);
    }
    return true;
}

void room_matchmaker::switch_readiness(const std::string &handle, bool force_leaving_battle) {
    std::unique_lock l(m_mutex_status);
    if (!m_users_in_rooms.count(handle)) {
        return;
    }
    auto &current_status = m_users_in_rooms.at(handle).status;
    switch (current_status) {
        case user_in_room::user_status::NOT_READY: {
            current_status = user_in_room::user_status::READY;
            auto &room = m_rooms.at(m_users_in_rooms.at(handle).room_id);
            auto &status_first = m_users_in_rooms.at(room[0]).status;
            auto &status_second = m_users_in_rooms.at(room[1]).status;
            if (room.size() == 2 && status_first == user_in_room::user_status::READY &&
                status_second == user_in_room::user_status::READY) {
                status_first = status_second = user_in_room::user_status::FIGHTING;
                game_handler::instance().add_game(
                    room[0], room[1], [](const std::string &handle_winner, const std::string &handle_loser) {
                        room_matchmaker::instance().switch_readiness(handle_winner, true);
                        room_matchmaker::instance().switch_readiness(handle_loser, true);
                    });
            } else {
                std::string other = (room[0] == handle ? room[1] : room[0]);
                message<messages_type> msg;
                msg.header.id = messages_type::ROOM_CHANGE_STATUS;
                server::instance().send_message(other, msg);
            }
            break;
        }
        case user_in_room::user_status::READY: {
            current_status = user_in_room::user_status::NOT_READY;
            auto &room = m_rooms.at(m_users_in_rooms.at(handle).room_id);
            std::string other = (room[0] == handle ? room[1] : room[0]);
            message<messages_type> msg;
            msg.header.id = messages_type::ROOM_CHANGE_STATUS;
            server::instance().send_message(other, msg);
        } break;
        case user_in_room::user_status::FIGHTING: {
            if (force_leaving_battle) {
                current_status = user_in_room::user_status::NOT_READY;
            }
        } break;
        default:
            assert(0);
    }
}
}  // namespace war_of_ages::server