#ifndef WAR_OF_AGES_SERVER_TOURNAMENT_H
#define WAR_OF_AGES_SERVER_TOURNAMENT_H

#include <vector>
#include "../../common/tournament/include/tournament.h"

namespace war_of_ages {

struct server_tournament : tournament {
private:
    std::vector<std::string> ready_to_play_participants;
    void post_add_participant(const std::string &handle) final;
    void post_add_result(const std::string &winner, const std::string &loser) final;
    void post_remove_participant(const std::string &handle) final;

    void match_participants();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SERVER_TOURNAMENT_H
