#ifndef CRANK_DATATABLES_HPP
#define CRANK_DATATABLES_HPP

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

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
    sf::Time                        fireInterval;
    std::vector<Direction>          directions;
};

struct ProjectileData
{
    int                             damage;
    float                           speed;
    Textures::ID                    texture;
};

struct PickupData
{
    std::function<void(Ship&)>      action;
    Textures::ID                    texture;
};

std::vector<ShipData>       initializeShipData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData>     initializePickupData();

#endif // CRANK_DATATABLES_HPP
