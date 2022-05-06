#ifndef WAR_OF_AGES_TOURNAMENT_HANDLER_H
#define WAR_OF_AGES_TOURNAMENT_HANDLER_H

#include <unordered_map>
#include "../include/server.h"
#include "../include/server_tournament.h"
#include <mutex>

namespace war_of_ages {

struct tournament_handler {
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, server_tournament> m_tournament;
    std::unordered_map<std::string, std::string> m_key_by_handle;

    tournament_handler() = default;

    static std::string gen_key();

public:
    tournament_handler(const tournament_handler &) = delete;
    tournament_handler(tournament_handler &&) = delete;
    tournament_handler &operator=(const tournament_handler &) = delete;
    tournament_handler &operator=(tournament_handler &&) = delete;
    ~tournament_handler() = default;

    void create(const std::string &handle, const std::string &tournament_name);
    void join(const std::string &handle, const std::string &key);
    void leave(const std::string &handle);
    void add_result(const std::string &winner, const std::string &loser);

    static tournament_handler &instance();
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_HANDLER_H