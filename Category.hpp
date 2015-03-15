#ifndef CRANK_CATEGORY_HPP
#define CRANK_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
    enum Type
    {
        None        = 0,
        Scene       = 1 << 0,
        PlayerShip  = 1 << 1,
        AlliedShip  = 1 << 2,
        EnemyShip   = 1 << 3
    };
}

#endif // CRANK_CATEGORY_HPP
