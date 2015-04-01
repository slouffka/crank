#include "Ship.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>


namespace
{
    const std::vector<ShipData> Table = initializeShipData();
}

Ship::Ship(Type type, const TextureManager& textures, FontManager& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mFireCommand()
, mMissileCommand()
, mFireCountdown(sf::Time::Zero)
, mIsFiring(false)
, mIsLaunchingMissile(false)
, mIsMarkedForRemoval(false)
, mFireRateLevel(1)
, mSpreadLevel(1)
, mMissileAmmo(2)
, mDropPickupCommand()
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mHealthDisplay(nullptr)
, mMissileDisplay(nullptr)
{
    centerOrigin(mSprite);

    mFireCommand.category = Category::SceneSpaceLayer;
    mFireCommand.action   = [this, &textures] (SceneNode& node, sf::Time)
    {
        createBullets(node, textures);
    };

    mMissileCommand.category = Category::SceneSpaceLayer;
    mMissileCommand.action   = [this, &textures] (SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };

    mDropPickupCommand.category = Category::SceneSpaceLayer;
    mDropPickupCommand.action   = [this, &textures] (SceneNode& node, sf::Time)
    {
        createPickup(node, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
    mHealthDisplay = healthDisplay.get();
    attachChild(std::move(healthDisplay));

    if (getCategory() == Category::PlayerShip)
    {
        std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
        missileDisplay->setPosition(0, 70);
        mMissileDisplay = missileDisplay.get();
        attachChild(std::move(missileDisplay));
    }

    updateTexts();
}

void Ship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Ship::updateCurrent(sf::Time frameTime, CommandQueue& commands)
{
    // Entity has been destroyed: Possibly drop pickup, mark for removal
    if (isDestroyed())
    {
        checkPickupDrop(commands);

        mIsMarkedForRemoval = true;
        return;
    }

    checkProjectileLaunch(frameTime, commands);

    // Update enemy movement pattern; apply velocity
    updateMovementPattern(frameTime);
    Entity::updateCurrent(frameTime, commands);

    // Update texts
    updateTexts();
}

unsigned int Ship::getCategory() const
{
    if (isAllied())
        return Category::PlayerShip;
    else
        return Category::EnemyShip;
}

sf::FloatRect Ship::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Ship::isMarkedForRemoval() const
{
    return mIsMarkedForRemoval;
}

bool Ship::isAllied() const
{
    return mType == Eagle;
}

float Ship::getMaxSpeed() const
{
    return Table[mType].speed;
}

void Ship::increaseFireRate()
{
    if (mFireRateLevel < 10)
        ++mFireRateLevel;
}

void Ship::increaseSpread()
{
    if (mSpreadLevel < 3)
        ++mSpreadLevel;
}

void Ship::collectMissiles(unsigned int count)
{
    mMissileAmmo += count;
}

void Ship::fire()
{
    // Only ships with fire interval != 0 are able to fire
    if (Table[mType].fireInterval != sf::Time::Zero)
        mIsFiring = true;
}

void Ship::launchMissile()
{
    if (mMissilesAmmo > 0)
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

void Ship::updateMovementPattern
