#include "Entity.hpp"

#include <cassert>


Entity::Entity(int hitpoints)
: mVelocity()
, mHitpoints(hitpoints)
{
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

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void repair(int hitpoints)
{
    assert(points > 0);

    mHitpoints += points;
}

void Entiry::damage(int points)
{
    assert(points > 0);

    mHitpoints -= points;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time frameTime)
{
    move(mVelocity * frameTime.asSeconds());
}
