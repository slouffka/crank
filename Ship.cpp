#include "Ship.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Textures::ID toTextureID(Ship::Type type)
{
    switch (type)
    {
        case Ship::Eagle:
            return Textures::Ship;

        case Ship::Raptor:
            return Textures::Ship;
    }
    return Textures::Ship;
}

Ship::Ship(Type type, const TextureManager& textures)
: mType(type)
, mSprite(textures.get(toTextureID(type)))
{
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Ship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
