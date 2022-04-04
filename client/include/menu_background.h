#ifndef WAR_OF_AGES_MENU_BACKGROUND_H
#define WAR_OF_AGES_MENU_BACKGROUND_H
#include <vector>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <cstddef>

namespace war_of_ages {

struct menu_background_handler {
private:
    const static int NUMBER_OF_PIECES = 18;
    std::vector<sf::Texture> m_pieces_texture;
    std::vector<std::vector<std::size_t>> m_current_layout;
    menu_background_handler();

public:
    const static int GRID_DIM_SIZE = 12;
    menu_background_handler(const menu_background_handler &) = delete;
    menu_background_handler(menu_background_handler &&) = default;
    menu_background_handler &operator=(const menu_background_handler &) = delete;
    menu_background_handler &operator=(menu_background_handler &&) = default;
    ~menu_background_handler() = default;

    [[nodiscard]] sf::Sprite get_piece(int i, int j) const;
    void change_layout();

    [[nodiscard]] static menu_background_handler &get_instance();
};

}

#endif  // WAR_OF_AGES_MENU_BACKGROUND_H
