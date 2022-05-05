#include "../include/room_matchmaker.h"
#include <algorithm>

namespace war_of_ages {
room_matchmaker &room_matchmaker::instance() {
    static room_matchmaker inst;
    return inst;
}

bool room_matchmaker::add_user_to_room(const std::string &user_id, const std::string &room_id) {
    std::unique_lock l(m_mutex_status);
    if (users_in_rooms.find(user_id) != users_in_rooms.end()) {
        // user must leave current room first
        return false;
    }
    auto &room = rooms[room_id];
    if (room.size() == 2) {
        return false;
    }
    room.push_back(user_id);
    users_in_rooms.insert({user_id, {room_id, user_in_room::user_status::NOT_READY}});
    return true;
}

bool room_matchmaker::remove_user_from_room(const std::string &user_id) {
    std::unique_lock l(m_mutex_status);
    auto it_user = users_in_rooms.find(user_id);
    if (it_user == users_in_rooms.end()) {
        return false;
    }
    if (it_user->second.status == user_in_room::user_status::FIGHTING) {
        // should finish the battle first
        return false;
    }
    auto &room = rooms[it_user->second.room_id];
    room.erase(std::remove(room.begin(), room.end(), user_id), room.end());
    users_in_rooms.erase(it_user);
    return true;
}

bool room_matchmaker::change_user_status(const std::string &user_id,
                                         user_in_room::user_status new_status,
                                         bool force_leaving_battle) {
    std::unique_lock l(m_mutex_status);
    auto it_user = users_in_rooms.find(user_id);
    if (it_user == users_in_rooms.end()) {
        return false;
    }
    auto &current_status = it_user->second.status;
    if (current_status == user_in_room::user_status::FIGHTING &&
        (!force_leaving_battle || new_status == user_in_room::user_status::READY)) {
        return false;
    }
    current_status = new_status;
    return true;
}
}  // namespace war_of_ages