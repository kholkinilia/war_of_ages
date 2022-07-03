#include "../include/background_handler.h"
#include <TGUI/Widgets/Group.hpp>
#include <random>
#include "../include/screen_handler.h"

namespace war_of_ages::client {

background_handler::background_handler() {
    m_pieces_texture.resize(NUMBER_OF_PIECES);
    for (int i = 0; i < NUMBER_OF_PIECES; i++) {
        m_pieces_texture[i].loadFromFile("../../client/resources/menu/background_pieces/" + std::to_string(i) +
                                         ".png");
    }
    m_layout_position.resize(GRID_DIM_SIZE, std::vector<sf::Vector2<float>>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_layout_position[i][j] = {static_cast<float>(i * PIECE_WIDTH),
                                       static_cast<float>(j * PIECE_HEIGHT)};
        }
    }
    m_buffer_position.resize(GRID_DIM_SIZE, std::vector<sf::Vector2<float>>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_buffer_position[i][j] = {static_cast<float>(i * PIECE_WIDTH),
                                       static_cast<float>(SCREEN_HEIGHT + j * PIECE_HEIGHT)};
        }
    }
    change_layout(m_buffer_layout);
    change_layout(m_current_layout);
}

void background_handler::change_layout(std::vector<std::vector<std::size_t>> &layout) noexcept {
    // TODO: take vector as an argument and use `change_layout()` in `start_animation()`
    layout.resize(GRID_DIM_SIZE, std::vector<std::size_t>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            layout[i][j] = get_rand_piece();
        }
    }
}

background_handler &background_handler::instance() {
    static background_handler handler;
    return handler;
}

void background_handler::print_background(const tgui::CanvasSFML::Ptr &canvas) {
    canvas->clear(sf::Color{50, 50, 50});
    float time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    update_animation(time);
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            sf::Sprite cur_piece{m_pieces_texture[m_current_layout[i][j]]};
            cur_piece.setPosition(m_layout_position[i][j]);
            canvas->draw(cur_piece);
            cur_piece = sf::Sprite{m_pieces_texture[m_buffer_layout[i][j]]};
            cur_piece.setPosition(m_buffer_position[i][j]);
            canvas->draw(cur_piece);
        }
    }
    canvas->display();
}

void background_handler::start_animation() {
    float time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    if (m_animation_processing) {
        return;
    }
    m_animation_processing = true;
    m_animation_start_time = time;
    m_buffer_layout.resize(GRID_DIM_SIZE, std::vector<std::size_t>(GRID_DIM_SIZE));
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_buffer_layout[i][j] = get_rand_piece();
            m_buffer_position[i][j] = {static_cast<float>(i * PIECE_WIDTH),
                                       static_cast<float>(SCREEN_HEIGHT + j * SCREEN_HEIGHT)};
            m_layout_position[i][j] = {static_cast<float>(i * PIECE_WIDTH),
                                       static_cast<float>(j * SCREEN_HEIGHT)};
        }
    }
}

std::size_t background_handler::get_rand_piece() noexcept {
    static std::mt19937 rnd(clock());
    static std::uniform_int_distribution<> distribution(0, NUMBER_OF_PIECES - 1);
    return distribution(rnd);
}

void background_handler::update_animation(float time) {
    if (!m_animation_processing) {
        return;
    }
    if (m_animation_start_time + ANIMATION_DURATION <= time) {
        m_animation_processing = false;
        m_animation_start_time = 0;
        m_current_layout = m_buffer_layout;
        for (int i = 0; i < GRID_DIM_SIZE; i++) {
            for (int j = 0; j < GRID_DIM_SIZE; j++) {
                m_layout_position[i][j] = {static_cast<float>(i * PIECE_WIDTH),
                                           static_cast<float>(j * PIECE_HEIGHT)};
                m_buffer_position[i][j] = {static_cast<float>(i * PIECE_WIDTH),
                                           static_cast<float>(SCREEN_HEIGHT + j * PIECE_HEIGHT)};
            }
        }
        return;
    }
    for (int i = 0; i < GRID_DIM_SIZE; i++) {
        for (int j = 0; j < GRID_DIM_SIZE; j++) {
            m_layout_position[i][j].y = j * PIECE_HEIGHT - get_shift(time);
            m_buffer_position[i][j].y = SCREEN_HEIGHT + j * PIECE_HEIGHT - get_shift(time);
        }
    }
}

float background_handler::get_shift(float time) const noexcept {
    float x = (time - m_animation_start_time) / ANIMATION_DURATION * 2;
    return SCREEN_HEIGHT * (3 * x * x - x * x * x) / 4;
}

void background_handler::init() {
    tgui::Group::Ptr background_group = tgui::Group::create();

    tgui::CanvasSFML::Ptr canvas = tgui::CanvasSFML::create();

    canvas->clear(sf::Color(50, 50, 50));
    canvas->setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    background_group->add(canvas, "background_canvas");

    screen_handler::instance().get_gui().add(background_group, "background_group");
}

}  // namespace war_of_ages::client
