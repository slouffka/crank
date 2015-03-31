#include "Ship.hpp"
#include "ShipData.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace
{
    const std::vector<ShipData> Table = initializeShipData();
}

Textures::ID toTextureID(Ship::Type type)
{
    switch (type)
    {
        case Ship::Eagle:
            return Textures::Eagle;

        case Ship::Raptor:
            return Textures::Raptor;
    }
    return Textures::Eagle;
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

unsigned int Ship::getCategory() const
{
    switch (mType)
    {
        case Eagle:
            return Category::PlayerShip;
        default:
            return Category::EnemyShip;
    }
}
