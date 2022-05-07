#ifndef WAR_OF_AGES_TOURNAMENT_HANDLER_H
#define WAR_OF_AGES_TOURNAMENT_HANDLER_H

#include <TGUI/Widgets/Grid.hpp>  // tgui::Gui, tgui::Grid
#include "../../common/tournament/include/tournament.h"

namespace war_of_ages {

struct tournament_handler : tournament {
private:
    tgui::Grid::Ptr m_grid;
    bool m_is_grid_updated = true;
    void post_add_participant(const std::string &handle) final;
    void post_add_result(const std::string &winner, const std::string &loser) final;
    void post_remove_participant(const std::string &handle) final;

public:
    void update_grid();
    void set_grid(tgui::Grid::Ptr &grid);
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_HANDLER_H
