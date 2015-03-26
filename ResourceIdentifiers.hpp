#ifndef CRANK_RESOURCEIDENTIFIERS_HPP
#define CRANK_RESOURCEIDENTIFIERS_HPP

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
        Eagle,
        Raptor,
        TitleScreen,
        ButtonNormal,
        ButtonSelected,
        ButtonPressed
    };
}

namespace Fonts
{
    enum ID
    {
        Main
    };
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceManager;

typedef ResourceManager<sf::Texture, Textures::ID> TextureManager;
typedef ResourceManager<sf::Font, Fonts::ID> FontManager;

#endif // CRANK_RESOURCEIDENTIFIERS_HPP
