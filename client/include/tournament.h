#ifndef WAR_OF_AGES_TOURNAMENT_H
#define WAR_OF_AGES_TOURNAMENT_H

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <mutex>
#include "screen_defines.h"

namespace war_of_ages {

struct tournament {
private:
    enum class result { NONE, VICTORY, DEFEAT };
    static inline const int WIN_POINTS = 1;
    int part_number = 0;
    std::vector<tgui::String> participants;
    std::map<tgui::String, int> id;
    std::vector<std::vector<result>> match_results;
    std::vector<int> sum;
    std::vector<int> place;
    std::mutex m;

    void update_places_lock_held();

public:
    void add_participant(const tgui::String &handle);

    void add_result(const tgui::String &winner, const tgui::String &loser);

    void remove_participant(const tgui::String &handle);

    void update_grid(const tgui::Grid::Ptr &grid);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_H
