#ifndef WAR_OF_AGES_SFML_PRINTER_H
#define WAR_OF_AGES_SFML_PRINTER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../../common/game_logic/include/game_state.h"

namespace war_of_ages {

struct sfml_printer {
private:
    sf::RenderWindow m_window;
    sf::View m_view;

    sfml_printer();

public:
    void init();
    void print_game(const std::pair<player_snapshot, player_snapshot> &snapshots);
    void handle_window_events();

    void set_view_center(int x, int y);
    [[nodiscard]] sf::RenderWindow &get_window();
    [[nodiscard]] sf::View &get_view();

    [[nodiscard]] static sfml_printer &instance();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SFML_PRINTER_H
