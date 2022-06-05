#ifndef WAR_OF_AGES_TOURNAMENT_H
#define WAR_OF_AGES_TOURNAMENT_H

#include <cstddef>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace war_of_ages {

enum class game_result : std::uint8_t { NONE, VICTORY, DEFEAT, PLAYING };

struct tournament_snapshot {
    std::string name;
    std::string key;
    std::vector<std::string> participants;
    std::vector<std::vector<game_result>> match_results;
};

struct tournament {
protected:
    static inline const int WIN_POINTS = 1;
    std::string m_name;
    std::string m_key;
    std::vector<std::string> m_participants;
    std::vector<std::vector<game_result>> m_match_results;
    std::vector<int> m_sum;
    std::vector<std::size_t> m_place;
    mutable std::mutex m_mutex;

    void update_places_lock_held();
    [[nodiscard]] std::size_t get_id(const std::string &handle) const noexcept;
    [[nodiscard]] bool is_participant(const std::string &handle) const;

protected:
    virtual void post_add_participant(const std::string &handle) = 0;
    virtual void post_add_result(const std::string &winner, const std::string &loser) = 0;
    virtual void post_remove_participant(const std::string &handle, std::size_t remove_id) = 0;
    virtual void post_match_participants(const std::string &handle1, const std::string &handle2) = 0;

public:
    tournament() = default;

    void add_participant(const std::string &handle);

    void add_result(const std::string &winner, const std::string &loser);

    void match_participants(const std::string &handle1, const std::string &handle2);

    void remove_participant(const std::string &handle);

    void set_name(const std::string &new_name);

    void set_key(const std::string &new_key);

    [[nodiscard]] tournament_snapshot get_snapshot() const;

    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] std::string get_key() const;

    virtual ~tournament() = default;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_H
