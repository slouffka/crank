#ifndef CRANK_UTILITY_HPP
#define CRANK_UTILITY_HPP

#include <SFML/Window/Keyboard.hpp>

#include <sstream>


namespace sf
{
    class Sprite;
    class Text;
}

// std::to_string do not work in MinGW so we need own implementation
// to support all platforms
template <typename T>
std::string toString(const T& value);

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include "Utility.inl"
#endif // CRANK_UTILITY_HPP
