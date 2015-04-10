#ifndef CRANK_PICKUP_HPP
#define CRANK_PICKUP_HPP

#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Ship;

class Pickup : public Entity
{
    public:
        enum Type
        {
            HealthRefill,
            MissileRefill,
            FireSpread,
            FireRate,
            TypeCount
        };


    public:
                                Pickup(Type type, const TextureManager& textures);

        virtual unsigned int    getCategory() const;
        virtual sf::FloatRect   getBoundingRect() const;

        void                    apply(Ship& player) const;


    protected:
        virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


    private:
        Type                    mType;
        sf::Sprite              mSprite;
};

#endif // CRANK_PICKUP_HPP
