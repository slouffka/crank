#ifndef CRANK_RESOURCE_IDENTIFIERS_HPP
#define CRANK_RESOURCE_IDENTIFIERS_HPP

// Forward declaration of SFML classes
namespace sf
{
    class Texture;
}

namespace Textures
{
    enum ID
    {
        Ship
    };
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceManager;

typedef ResourceManager<sf::Texture, Textures::ID> TextureManager;

#endif // CRANK_RESOURCE_IDENTIFIERS_HPP
