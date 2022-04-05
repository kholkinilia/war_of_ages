#ifndef WAR_OF_AGES_MENU_BACKGROUND_H
#define WAR_OF_AGES_MENU_BACKGROUND_H
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <cstddef>
#include <ctime>
#include <random>
#include <vector>

namespace war_of_ages {

struct menu_background_handler {
private:
    const static int NUMBER_OF_PIECES = 18;
    const static int GRID_DIM_SIZE = 12;
    const static int SCREEN_WIDTH = 1920;
    const static int SCREEN_HEIGHT = 1080;
    const static int PIECE_WIDTH = SCREEN_WIDTH / GRID_DIM_SIZE;
    const static int PIECE_HEIGHT = SCREEN_HEIGHT / GRID_DIM_SIZE;
    constexpr const static float ANIMATION_DURATION = 0.2;
    std::vector<sf::Texture> m_pieces_texture;
    std::vector<std::vector<std::size_t>> m_current_layout;
    std::vector<std::vector<sf::Vector2<float>>> m_layout_position;

    std::vector<std::vector<std::size_t>> m_animation_layout;
    std::vector<std::vector<sf::Vector2<float>>> m_buffer_position;
    float m_animation_start_time = 0;
    bool m_animation_processing = false;

    menu_background_handler();
    [[nodiscard]] static std::size_t get_rand_piece() noexcept;

public:
    menu_background_handler(const menu_background_handler &) = delete;
    menu_background_handler(menu_background_handler &&) = default;
    menu_background_handler &operator=(const menu_background_handler &) = delete;
    menu_background_handler &operator=(menu_background_handler &&) = default;
    ~menu_background_handler() = default;

    void print_background(sf::RenderWindow &window, float time);
    void start_animation(float time);
    void update_animation(float time);

    static void change_layout(std::vector<std::vector<std::size_t>> &layout) noexcept;
    [[nodiscard]] static menu_background_handler &get_instance();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_MENU_BACKGROUND_H
