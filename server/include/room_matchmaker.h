#ifndef WAR_OF_AGES_ROOM_MATCHMAKER_H
#define WAR_OF_AGES_ROOM_MATCHMAKER_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace war_of_ages {

struct room_matchmaker {
    static room_matchmaker &instance();
    ~room_matchmaker() = default;
    room_matchmaker(const room_matchmaker &other) = delete;
    room_matchmaker(room_matchmaker &&other) = delete;
    room_matchmaker &operator=(const room_matchmaker &other) = delete;
    room_matchmaker &operator=(room_matchmaker &&other) = delete;

    bool add_user_to_room(const std::string &handle, const std::string &room_id);
    bool remove_user_from_room(const std::string &handle);
    void switch_readiness(const std::string &handle, bool force_leaving_battle = false);

private:
    struct user_in_room {
        std::string room_id;
        enum class user_status {
            NOT_READY,
            READY,
            FIGHTING,
        } status;
    };

    room_matchmaker() = default;
    [[nodiscard]] bool user_exists(const std::string &handle) const;

    std::unordered_map<std::string, std::vector<std::string>> m_rooms;  // {room_id, user_id}
    std::unordered_map<std::string, user_in_room> m_users_in_rooms;
    mutable std::mutex m_mutex_status;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_ROOM_MATCHMAKER_H
