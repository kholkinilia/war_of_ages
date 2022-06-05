#ifndef WAR_OF_AGES_BACKGROUND_HANDLER_H
#define WAR_OF_AGES_BACKGROUND_HANDLER_H

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <cstddef>
#include <ctime>
#include <random>
#include <vector>

namespace war_of_ages::client {

struct background_handler {
private:
    const static int NUMBER_OF_PIECES = 18;
    const static int GRID_DIM_SIZE = 12;
    const static int SCREEN_WIDTH = 1920;
    const static int SCREEN_HEIGHT = 1080;
    const static int PIECE_WIDTH = SCREEN_WIDTH / GRID_DIM_SIZE;
    const static int PIECE_HEIGHT = SCREEN_HEIGHT / GRID_DIM_SIZE;
    constexpr const static float ANIMATION_DURATION = 0.5;
    std::vector<sf::Texture> m_pieces_texture;
    std::vector<std::vector<std::size_t>> m_current_layout;
    std::vector<std::vector<sf::Vector2<float>>> m_layout_position;

    std::vector<std::vector<std::size_t>> m_buffer_layout;
    std::vector<std::vector<sf::Vector2<float>>> m_buffer_position;
    float m_animation_start_time = 0;
    bool m_animation_processing = false;

    background_handler();
    [[nodiscard]] float get_shift(float time) const noexcept;
    [[nodiscard]] static std::size_t get_rand_piece() noexcept;

    void update_animation(float time);

public:
    background_handler(const background_handler &) = delete;
    background_handler(background_handler &&) = delete;
    background_handler &operator=(const background_handler &) = delete;
    background_handler &operator=(background_handler &&) = delete;
    ~background_handler() = default;

    static void init();

    void print_background(const tgui::CanvasSFML::Ptr &canvas);
    void start_animation();

    static void change_layout(std::vector<std::vector<std::size_t>> &layout) noexcept;
    [[nodiscard]] static background_handler &instance();
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_BACKGROUND_HANDLER_H
