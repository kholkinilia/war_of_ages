#include "../include/menu_background.h"
#include <random>

namespace war_of_ages {

menu_background_handler::menu_background_handler() {
    m_pieces_texture.resize(NUMBER_OF_PIECES);
    for (int i = 0; i < NUMBER_OF_PIECES; i++) {
        m_pieces_texture[i].loadFromFile("../client/resources/menu/background_pieces/" + std::to_string(i) +
                                         ".png");
    }
    m_layout_position.resize(GRID_DIM_SIZE, std::vector<sf::Vector2<float>>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_layout_position[i][j] = {1.f * i * PIECE_WIDTH, 1.f * j * PIECE_HEIGHT};
        }
    }
    m_buffer_position.resize(GRID_DIM_SIZE, std::vector<sf::Vector2<float>>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_buffer_position[i][j] = {1.f * i * PIECE_WIDTH, SCREEN_HEIGHT + 1.f * j * PIECE_HEIGHT};
        }
    }
    change_layout(m_animation_layout);
    change_layout(m_current_layout);
}

void menu_background_handler::change_layout(std::vector<std::vector<std::size_t>> &layout) noexcept {
    // TODO: take vector as an argument and use `change_layout()` in `start_animation()`
    layout.resize(GRID_DIM_SIZE, std::vector<std::size_t>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            layout[i][j] = get_rand_piece();
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

void menu_background_handler::print_background(sf::RenderWindow &window, float time) {
    update_animation(time);
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            sf::Sprite cur_piece{m_pieces_texture[m_current_layout[i][j]]};
            cur_piece.setPosition(m_layout_position[i][j]);
            window.draw(cur_piece);
            cur_piece = sf::Sprite{m_pieces_texture[m_animation_layout[i][j]]};
            cur_piece.setPosition(m_buffer_position[i][j]);
            window.draw(cur_piece);
        }
    }
}

void menu_background_handler::start_animation(float time) {
    if (m_animation_processing) {
        return;
    }
    m_animation_processing = true;
    m_animation_start_time = time;
    m_animation_layout.resize(GRID_DIM_SIZE, std::vector<std::size_t>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_animation_layout[i][j] = get_rand_piece();
            m_buffer_position[i][j] = {1.f * i * PIECE_WIDTH, 1.f * SCREEN_HEIGHT + j * SCREEN_HEIGHT};
            m_layout_position[i][j] = {1.f * i * PIECE_WIDTH, 1.f * j * SCREEN_HEIGHT};
        }
    }
}

std::size_t menu_background_handler::get_rand_piece() noexcept {
    static std::mt19937 rnd(clock());
    return rnd() % NUMBER_OF_PIECES;
}

void menu_background_handler::update_animation(float time) {
    if (!m_animation_processing) {
        return;
    }
    if (m_animation_start_time + ANIMATION_DURATION <= time) {
        m_animation_processing = false;
        m_animation_start_time = 0;
        m_current_layout = m_animation_layout;
        for (int i = 0; i < GRID_DIM_SIZE; i++) {
            for (int j = 0; j < GRID_DIM_SIZE; j++) {
                m_layout_position[i][j] = {1.f * i * PIECE_WIDTH, 1.f * j * PIECE_HEIGHT};
                m_buffer_position[i][j] = {1.f * i * PIECE_WIDTH, 1.f * SCREEN_HEIGHT + j * PIECE_HEIGHT};
            }
        }
        return;
    }
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_layout_position[i][j].y =
                j * PIECE_HEIGHT - SCREEN_HEIGHT * (time - m_animation_start_time) / ANIMATION_DURATION;
            m_buffer_position[i][j].y = SCREEN_HEIGHT + j * PIECE_HEIGHT -
                                        SCREEN_HEIGHT * (time - m_animation_start_time) / ANIMATION_DURATION;
        }
    }
}

}  // namespace war_of_ages
