#include "../include/room_matchmaker.h"
#include <algorithm>
#include <cassert>

namespace war_of_ages {
room_matchmaker &room_matchmaker::instance() {
    static room_matchmaker inst;
    return inst;
}

bool room_matchmaker::user_exists(const std::string &handle) const {
    std::unique_lock l(m_mutex_status);
    return users_in_rooms.find(handle) != users_in_rooms.end();
}

bool room_matchmaker::add_user_to_room(const std::string &handle, const std::string &room_id) {
    std::unique_lock l(m_mutex_status);
    if (user_exists(handle)) {
        return false;
    }
    auto &room = rooms[room_id];
    if (room.size() == 2) {
        return false;
    }
    room.push_back(handle);
    users_in_rooms.insert({handle, {room_id, user_in_room::user_status::NOT_READY}});
    return true;
}

bool room_matchmaker::remove_user_from_room(const std::string &handle) {
    if (!user_exists(handle)) {
        return false;
    }
    std::unique_lock l(m_mutex_status);
    auto it_user = users_in_rooms.find(handle);
    if (it_user->second.status == user_in_room::user_status::FIGHTING) {
        // should finish the battle first
        return false;
    }
    auto &room = rooms[it_user->second.room_id];
    room.erase(std::remove(room.begin(), room.end(), handle), room.end());
    users_in_rooms.erase(it_user);
    return true;
}

void room_matchmaker::switch_readiness(const std::string &handle, bool force_leaving_battle) {
    if (!user_exists(handle)) {
        return;
    }
    std::unique_lock l(m_mutex_status);
    auto &current_status = users_in_rooms.at(handle).status;
    switch (current_status) {
        case user_in_room::user_status::NOT_READY:
            current_status = user_in_room::user_status::READY;
            break;
        case user_in_room::user_status::READY:
            current_status = user_in_room::user_status::NOT_READY;
            break;
        case user_in_room::user_status::FIGHTING: {
            if (force_leaving_battle) {
                current_status = user_in_room::user_status::NOT_READY;
            }
        } break;
        default:
            assert(0);
    }
}
}  // namespace war_of_ages