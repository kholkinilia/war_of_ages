#ifndef WAR_OF_AGES_TOURNAMENT_H
#define WAR_OF_AGES_TOURNAMENT_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace war_of_ages {

struct tournament {
protected:
    enum class result { NONE, VICTORY, DEFEAT };
    static inline const int WIN_POINTS = 1;
    std::string m_name;
    std::string m_key;
    int m_part_number = 0;
    std::vector<std::string> m_participants;
    std::map<std::string, int> m_id;
    std::vector<std::vector<result>> m_match_results;
    std::vector<int> m_sum;
    std::vector<int> m_place;
    mutable std::mutex m_mutex;

    void update_places_lock_held();

protected:
    virtual void post_add_participant(const std::string &handle) = 0;
    virtual void post_add_result(const std::string &winner, const std::string &loser) = 0;
    virtual void post_remove_participant(const std::string& handle) = 0;

public:
    void add_participant(const std::string &handle);

    void add_result(const std::string &winner, const std::string &loser);

    void remove_participant(const std::string &handle);

    void set_name(const std::string &new_name);

    void set_key(const std::string &new_key);

    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] std::string get_key() const;

    virtual ~tournament() = default;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_H
