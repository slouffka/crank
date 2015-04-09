#ifndef CRANK_CATEGORY_HPP
#define CRANK_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
    enum Type
    {
        None                = 0,
        SceneSpaceLayer     = 1 << 0,
        PlayerShip          = 1 << 1,
        AlliedShip          = 1 << 2,
        EnemyShip           = 1 << 3,
        Pickup              = 1 << 4,
        AlliedProjectile    = 1 << 5,
        EnemyProjectile     = 1 << 6,
        ParticleSystem      = 1 << 7,

        Ship = PlayerShip | AlliedShip | EnemyShip,
        Projectile = AlliedProjectile | EnemyProjectile
    };
}

#endif // CRANK_CATEGORY_HPP
