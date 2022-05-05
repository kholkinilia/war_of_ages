#ifndef WAR_OF_AGES_ROOM_MATCHMAKER_H
#define WAR_OF_AGES_ROOM_MATCHMAKER_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace war_of_ages {

struct user_in_room {
    std::string room_id;
    enum class user_status {
        NOT_READY,
        READY,
        FIGHTING,
    } status;
};

struct room_matchmaker {
    static room_matchmaker &instance();
    ~room_matchmaker() = default;
    room_matchmaker(const room_matchmaker &other) = delete;
    room_matchmaker(room_matchmaker &&other) = delete;
    room_matchmaker &operator=(const room_matchmaker &other) = delete;
    room_matchmaker &operator=(room_matchmaker &&other) = delete;

    bool add_user_to_room(const std::string &user_id, const std::string &room_id);
    bool remove_user_from_room(const std::string &user_id);
    bool change_user_status(const std::string &user_id,
                            user_in_room::user_status new_status,
                            bool force_leaving_battle = false);

private:
    room_matchmaker() = default;

    std::unordered_map<std::string, std::vector<std::string>> rooms;  // {room_id, user_id}
    std::unordered_map<std::string, user_in_room> users_in_rooms;
    std::mutex m_mutex_status;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_ROOM_MATCHMAKER_H
