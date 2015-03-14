#ifndef CRANK_SHIP_HPP
#define CRANK_SHIP_HPP

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Ship : public Entity
{
    public:
        enum Type
        {
            Eagle,
            Raptor
        };


    public:
                            Ship(Type type, const TextureManager& textures);


    private:
        virtual void        drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


    private:
        Type                mType;
        sf::Sprite          mSprite;
};

#endif // CRANK_SHIP_HPP
