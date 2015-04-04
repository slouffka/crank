#include "DataTables.hpp"
#include "Ship.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<ShipData> initializeShipData()
{
    std::vector<ShipData> data(Ship::TypeCount);

    data[Ship::Eagle].hitpoints = 100;
    data[Ship::Eagle].speed = 200.f;
    data[Ship::Eagle].fireInterval = sf;:seconds(1);
    data[Ship::Eagle].texture = Textures::Eagle;

    data[Ship::Raptor].hitpoints = 20;
    data[Ship::Raptor].speed = 80.f;
    data[Ship::Raptor].texture = Textures::Raptor;
    data[Ship::Raptor].directions.push_back(Direction(+45.f, 80.f));
    data[Ship::Raptor].directions.push_back(Direction(-45.f, 160.f));
    data[Ship::Raptor].directions.push_back(Direction(+45.f, 80.f));
    data[Ship::Raptor].fireInterval = sf::Time::Zero;

    data[Ship::Avenger].hitpoints = 40;
    data[Ship::Avenger].speed = 50.f;
    data[Ship::Avenger].texture = Textures::Avenger;
    data[Ship::Avenger].directions.push_back(Direction(+45.f,  50.f));
    data[Ship::Avenger].directions.push_back(Direction(  0.f,  50.f));
    data[Ship::Avenger].directions.push_back(Direction(-45.f, 100.f));
    data[Ship::Avenger].directions.push_back(Direction(  0.f,  50.f));
    data[Ship::Avenger].directions.push_back(Direction(+45.f,  50.f));
    data[Ship::Avenger].fireInterval = sf::seconds(2);

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 300.f;
    data[Projectile::AlliedBullet].texture = Textures::Bullet;

    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 300.f;
    data[Projectile::EnemyBullet].texture = Textures::Bullet;

    data[Projectile::Missile].damage = 200;
    data[Projectile::Missile].speed = 150.f;
    data[Projectile::Missile].texture = Textures::Missile;

    return data;
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::HealthRefill].texture = Textures::HealthRefill;
    data[Pickup::HealthRefill].action = [] (Ship& a) { a.repair(25); };

    data[Pickup::MissileRefill].texture = Textures::MissileRefill;
    data[Pickup::MissileRefill].action = std::bind(&Ship::collectMissiles, _1, 3);

    data[Pickup::FireSpread].texture = Textures::FireSpread;
    data[Pickup::FireSpread].action = std::bind(&Ship::increaseSpread, _1);

    data[Pickup::FireRate].texture = Textures::FireRate;
    data[Pickup::FireRate].action = std::bind(&Ship::increaseFireRate, _1);

    return data;
}
