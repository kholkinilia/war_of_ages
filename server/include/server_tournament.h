#ifndef WAR_OF_AGES_SERVER_TOURNAMENT_H
#define WAR_OF_AGES_SERVER_TOURNAMENT_H

#include <vector>
#include "server.h"
#include "tournament.h"

namespace war_of_ages::server {

struct server_tournament : tournament {
private:
    std::vector<bool> m_is_playing;
    std::vector<bool> m_is_in_tournament;

    void post_add_participant(const std::string &handle) final;
    void post_add_result(const std::string &winner, const std::string &loser) final;
    void post_remove_participant(const std::string &handle, std::size_t remove_id) final;
    void post_match_participants(const std::string &handle1, const std::string &handle2) final;

    void match_participants_lock_held();
};

}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_SERVER_TOURNAMENT_H
