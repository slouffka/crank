#ifndef CRANK_RESOURCE_IDENTIFIERS_HPP
#define CRANK_RESOURCE_IDENTIFIERS_HPP

// Forward declaration of SFML classes
namespace sf
{
    class Texture;
    class Font;
}

namespace Textures
{
    enum ID
    {
        Background,
        Ship
    };
}

namespace Fonts
{
    enum ID
    {
        Arcade
    };
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceManager;

typedef ResourceManager<sf::Texture, Textures::ID> TextureManager;
typedef ResourceManager<sf::Font, Fonts::ID> FontManager;

#endif // CRANK_RESOURCE_IDENTIFIERS_HPP
