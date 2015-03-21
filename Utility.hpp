#ifndef CRANK_UTILITY_HPP
#define CRANK_UTILITY_HPP

#include <sstream>


namespace sf
{
    class Sprite;
    class Text;
}

// std::to_string do not work in MinGW so we need own implementation
template <typename T>
std::string toString(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include "Utility.inl"
#endif // CRANK_UTILITY_HPP
