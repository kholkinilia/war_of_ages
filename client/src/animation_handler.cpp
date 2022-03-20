#include "../include/animation_handler.h"
namespace war_of_ages {

animation_handler::animation_handler(const std::string &texture_filename,
                                     std::vector<float> &&time_period,
                                     int rows,
                                     int columns,
                                     int each_width_pxls,
                                     int each_height_pxls)
    : m_time_period(std::move(time_period)),
      m_rows(rows),
      m_columns(columns),
      m_each_width_pxls(each_width_pxls),
      m_each_height_pxls(each_height_pxls) {
    assert(m_time_period.size() == rows);
    m_texture.loadFromFile(texture_filename);
    m_sprite.setTexture(m_texture);
}

sf::Sprite animation_handler::get_sprite(int line, float time) const noexcept {
    sf::Sprite res = m_sprite;
    time -= time / m_time_period[line] * m_time_period[line];
    res.setTextureRect(sf::IntRect(
        line * m_each_height_pxls,
        static_cast<int>((time / m_time_period[line]) * static_cast<float>(m_columns)) % m_columns,
        m_each_width_pxls, m_each_height_pxls));
    return res;
}

}  // namespace war_of_ages