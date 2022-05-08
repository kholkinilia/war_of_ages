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
    sf::Vector2f m_game_view_center;

    sfml_printer();

public:
    void init();
    void print_game(const std::pair<player_snapshot, player_snapshot> &snapshots);
    void handle_window_events();

    void update(sf::Sprite background);
    void draw();

    void set_game_view_center(sf::Vector2f new_center) noexcept;
    [[nodiscard]] sf::Vector2f get_game_view_center() const noexcept;
    [[nodiscard]] sf::RenderWindow &get_window() noexcept;
    [[nodiscard]] sf::View &get_view() noexcept;

    [[nodiscard]] static sfml_printer &instance();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SFML_PRINTER_H
