#ifndef CRANK_ENTITY_HPP
#define CRANK_ENTITY_HPP

#include "SceneNode.hpp"


class Entity : public SceneNode
{
    public:
        explicit            Entity(int hitpoints);

        void                rapair(int points);
        void                damage(int points);
        void                destroy();

        int                 getHitpoints() const;
        int                 isDestroyed() const;

        void                setVelocity(sf::Vector2f velocity);
        void                setVelocity(float vx, float vy);
        void                accelerate(sf::Vector2f velocity);
        void                accelerate(float vx, float vy);
        sf::Vector2f        getVelocity() const;


    private:
        virtual void        updateCurrent(sf::Time frameTime);


    private:
        int                 mHitpoints;
        sf::Vector2f        mVelocity;
};

#endif // CRANK_ENTITY_HPP
