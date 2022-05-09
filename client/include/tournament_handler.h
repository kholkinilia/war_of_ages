#ifndef WAR_OF_AGES_TOURNAMENT_HANDLER_H
#define WAR_OF_AGES_TOURNAMENT_HANDLER_H

#include <TGUI/Widgets/Grid.hpp>  // tgui::Gui, tgui::Grid
#include <TGUI/Widgets/Label.hpp>
#include "../../common/tournament/include/tournament.h"

namespace war_of_ages::client {

struct tournament_handler : tournament {
private:
    tournament_handler() = default;

    tgui::Grid::Ptr m_grid;
    tgui::Label::Ptr m_name_label;
    tgui::Label::Ptr m_key_label;
    bool m_is_grid_updated = true;
    void post_add_participant(const std::string &handle) final;
    void post_add_result(const std::string &winner, const std::string &loser) final;
    void post_remove_participant(const std::string &handle) final;

public:
    tournament_handler(const tournament_handler &) = delete;
    tournament_handler(tournament_handler &&) = delete;
    tournament_handler &operator=(const tournament_handler &) = delete;
    tournament_handler &operator=(tournament_handler &&) = delete;

    void update_grid();
    void set_grid(tgui::Grid::Ptr &grid);
    void set_name_label(tgui::Label::Ptr &name_label);
    void set_key_label(tgui::Label::Ptr &key_label);
    void set_tournament(const tournament_snapshot &snapshot);

    [[nodiscard]] static tournament_handler &instance();
};
}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_TOURNAMENT_HANDLER_H
