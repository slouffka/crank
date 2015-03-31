#include "Entity.hpp"

Entity::Entity(int hitpoints)
: mHitpoints(hitpoints)
{
}

void repair(int hitpoints)
{
    if (mHitpoints + hitpoints <= MAX_HITPOINTS)
        mHitpoints += hitpoints;
    else
        mHitpoints = MAX_HITPOINTS;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

void Entity::updateCurrent(sf::Time frameTime)
{
    move(mVelocity * frameTime.asSeconds());
}
