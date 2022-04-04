#include "../include/menu_background.h"
#include <random>

namespace war_of_ages {

menu_background_handler::menu_background_handler() {
    m_current_layout.resize(GRID_DIM_SIZE, std::vector<std::size_t>(GRID_DIM_SIZE));
    m_pieces_texture.resize(NUMBER_OF_PIECES);
    for (int i = 0; i < NUMBER_OF_PIECES; i++) {
        m_pieces_texture[i].loadFromFile("../client/resources/menu/background_pieces/" + std::to_string(i) +
                                         ".png");
    }
    change_layout();
}

void menu_background_handler::change_layout() {
    std::mt19937 rnd(clock());
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_current_layout[i][j] = rnd() % NUMBER_OF_PIECES;
        }
    }
}

sf::Sprite menu_background_handler::get_piece(int i, int j) const {
    return sf::Sprite(m_pieces_texture[m_current_layout[i][j]]);
}
menu_background_handler &menu_background_handler::get_instance() {
    static menu_background_handler handler;
    return handler;
}

}  // namespace war_of_ages
