#ifndef WAR_OF_AGES_ANIMATION_HANDLER_H
#define WAR_OF_AGES_ANIMATION_HANDLER_H

#include <string>
#include <tgui/Backend/SFML-Graphics.hpp>
#include <vector>

namespace war_of_ages {

struct animation_handler {
private:
    sf::Texture m_texture;
    std::vector<float> m_time_period;
    [[maybe_unused]] int m_rows = 1;   // TODO: think if it will be needed for anything in future
    int m_columns = 1;
    int m_each_width_pxls = 0;
    int m_each_height_pxls = 0;

public:
    animation_handler(const std::string &texture_filename,
                      std::vector<float> time_period,
                      int rows,
                      int columns,
                      int each_width_pxls,
                      int each_height_pxls);
    animation_handler() = default;

    [[nodiscard]] sf::Sprite get_sprite(int line, float time) noexcept;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_ANIMATION_HANDLER_H
