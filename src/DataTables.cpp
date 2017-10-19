#include "DataTables.hpp"
#include "Ship.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Particle.hpp"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<ShipData> initializeShipData()
{
    std::vector<ShipData> data(Ship::TypeCount);

    data[Ship::Eagle].hitpoints = 100;
    data[Ship::Eagle].speed = 200.f;
    data[Ship::Eagle].fireInterval = sf::seconds(1);
    data[Ship::Eagle].texture = Textures::Entities;
    data[Ship::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
    data[Ship::Eagle].hasRollAnimation = true;

    data[Ship::Raptor].hitpoints = 20;
    data[Ship::Raptor].speed = 80.f;
    data[Ship::Raptor].texture = Textures::Entities;
    data[Ship::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
    data[Ship::Raptor].directions.push_back(Direction(+45.f, 80.f));
    data[Ship::Raptor].directions.push_back(Direction(-45.f, 160.f));
    data[Ship::Raptor].directions.push_back(Direction(+45.f, 80.f));
    data[Ship::Raptor].fireInterval = sf::Time::Zero;
    data[Ship::Raptor].hasRollAnimation = false;

    data[Ship::Avenger].hitpoints = 40;
    data[Ship::Avenger].speed = 50.f;
    data[Ship::Avenger].texture = Textures::Entities;
    data[Ship::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
    data[Ship::Avenger].directions.push_back(Direction(+45.f,  50.f));
    data[Ship::Avenger].directions.push_back(Direction(  0.f,  50.f));
    data[Ship::Avenger].directions.push_back(Direction(-45.f, 100.f));
    data[Ship::Avenger].directions.push_back(Direction(  0.f,  50.f));
    data[Ship::Avenger].directions.push_back(Direction(+45.f,  50.f));
    data[Ship::Avenger].fireInterval = sf::seconds(2);
    data[Ship::Avenger].hasRollAnimation = false;

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 300.f;
    data[Projectile::AlliedBullet].texture = Textures::Entities;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 300.f;
    data[Projectile::EnemyBullet].texture = Textures::Entities;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);

    data[Projectile::Missile].damage = 200;
    data[Projectile::Missile].speed = 150.f;
    data[Projectile::Missile].texture = Textures::Entities;
    data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

    return data;
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::HealthRefill].texture = Textures::Entities;
    data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
    data[Pickup::HealthRefill].action = [] (Ship& s) { s.repair(25); };

    data[Pickup::MissileRefill].texture = Textures::Entities;
    data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
    data[Pickup::MissileRefill].action = std::bind(&Ship::collectMissiles, _1, 3);

    data[Pickup::FireSpread].texture = Textures::Entities;
    data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
    data[Pickup::FireSpread].action = std::bind(&Ship::increaseSpread, _1);

    data[Pickup::FireRate].texture = Textures::Entities;
    data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
    data[Pickup::FireRate].action = std::bind(&Ship::increaseFireRate, _1);

    return data;
}

std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Propellant].color = sf::Color(255, 200, 50);
    data[Particle::Propellant].lifetime = sf::seconds(0.5f);

    data[Particle::Smoke].color = sf::Color(50, 50, 50);
    data[Particle::Smoke].lifetime = sf::seconds(2.f);

    return data;
}
