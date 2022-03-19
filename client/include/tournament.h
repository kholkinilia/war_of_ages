#ifndef WAR_OF_AGES_TOURNAMENT_H
#define WAR_OF_AGES_TOURNAMENT_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui, tgui::String
#include <TGUI/Widgets/Grid.hpp>           // tgui::Grid
#include <map>
#include <mutex>
#include <vector>

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
    bool is_grid_updated = true;

    void update_places_lock_held();

public:
    void add_participant(const tgui::String &handle);

    void add_result(const tgui::String &winner, const tgui::String &loser);

    void remove_participant(const tgui::String &handle);

    void update_grid(const tgui::Grid::Ptr &grid);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_TOURNAMENT_H
