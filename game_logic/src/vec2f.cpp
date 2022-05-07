#include "../include/vec2f.h"
#include <iostream>

namespace war_of_ages {
vec2f::vec2f() noexcept : sf::Vector2f{} {
}
vec2f::vec2f(const sf::Vector2f &vec) noexcept : sf::Vector2f{vec} {
}
vec2f::vec2f(float x, float y) noexcept : sf::Vector2f{x, y} {
}
vec2f::vec2f(int x, int y) noexcept : sf::Vector2f{static_cast<float>(x), static_cast<float>(y)} {
}

vec2f vec2f::normalize() const noexcept {
    return *this / sqrtf(x * x + y * y);
}

std::ostream &operator<<(std::ostream &os, const vec2f &vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

}  // namespace war_of_ages