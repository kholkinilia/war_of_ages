#ifndef WAR_OF_AGES_PRINTABLE_TOURNAMENT_H
#define WAR_OF_AGES_PRINTABLE_TOURNAMENT_H

#include <TGUI/Widgets/Grid.hpp>  // tgui::Gui, tgui::Grid
#include "../../common/tournament/include/tournament.h"

namespace war_of_ages {

struct printable_tournament : tournament {
public:
    void update_grid(const tgui::Grid::Ptr &grid);
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_PRINTABLE_TOURNAMENT_H
