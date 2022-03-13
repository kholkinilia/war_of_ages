#ifndef WAR_OF_AGES_VEC2_H
#define WAR_OF_AGES_VEC2_H

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iosfwd>

namespace war_of_ages {
struct vec2f : public sf::Vector2f {
    vec2f() noexcept;
    vec2f(const sf::Vector2f &vec) noexcept;
    vec2f(float x, float y) noexcept;

    [[nodiscard]] vec2f normalize() const noexcept;
};

std::ostream &operator<<(std::ostream &os, const vec2f &vec);

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_VEC2_H
