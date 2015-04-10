#ifndef CRANK_DATATABLES_HPP
#define CRANK_DATATABLES_HPP

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>


class Ship;

struct Direction
{
    Direction(float angle, float distance)
    : angle(angle)
    , distance(distance)
    {
    }

    float angle;
    float distance;
};

struct ShipData
{
    int                             hitpoints;
    float                           speed;
    Textures::ID                    texture;
    sf::IntRect                     textureRect;
    sf::Time                        fireInterval;
    std::vector<Direction>          directions;
    bool                            hasRollAnimation;
};

struct ProjectileData
{
    int                             damage;
    float                           speed;
    Textures::ID                    texture;
    sf::IntRect                     textureRect;
};

struct PickupData
{
    std::function<void(Ship&)>      action;
    Textures::ID                    texture;
    sf::IntRect                     textureRect;
};

struct ParticleData
{
    sf::Color                       color;
    sf::Time                        lifetime;
};

std::vector<ShipData>       initializeShipData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData>     initializePickupData();
std::vector<ParticleData>   initializeParticleData();

#endif // CRANK_DATATABLES_HPP
