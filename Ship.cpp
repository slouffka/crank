#include "Ship.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "SoundNode.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>


using namespace std::placeholders;

namespace
{
    const std::vector<ShipData> Table = initializeShipData();
}

Ship::Ship(Type type, const TextureManager& textures, const FontManager& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
, mExplosion(textures.get(Textures::Explosion))
, mFireCommand()
, mMissileCommand()
, mFireCountdown(sf::Time::Zero)
, mIsFiring(false)
, mIsLaunchingMissile(false)
, mShowExplosion(true)
, mPlayedExplosionSound(false)
, mSpawnedPickup(false)
, mFireRateLevel(1)
, mSpreadLevel(1)
, mMissileAmmo(2)
, mDropPickupCommand()
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mHealthDisplay(nullptr)
, mMissileDisplay(nullptr)
{
    mExplosion.setFrameSize(sf::Vector2i(256, 256));
    mExplosion.setNumFrames(16);
    mExplosion.setDuration(sf::seconds(1));

    centerOrigin(mSprite);
    centerOrigin(mExplosion);

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
    if (isDestroyed() && mShowExplosion)
        target.draw(mExplosion, states);
    else
        target.draw(mSprite, states);
}

void Ship::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // Update texts and roll animation
    updateTexts();
    updateRollAnimation();

    // Entity has been destroyed: Possibly drop pickup, mark for removal
    if (isDestroyed())
    {
        checkPickupDrop(commands);
        mExplosion.update(dt);

        // Play explosion sound only once
        if (!mPlayedExplosionSound)
        {
            SoundEffect::ID soundEffect = (randomInt(2) == 0) ? soundEffect::Explosion1 : SoundEffect::Explosion2;
            playLocalSound(commands, soundEffect);

            mPlayedExplosionSound = true;
        }
        return;
    }

    // Check if bullets or missiles are fired
    checkProjectileLaunch(dt, commands);

    // Update enemy movement pattern; apply velocity
    updateMovementPattern(dt);
    Entity::updateCurrent(dt, commands);
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
    return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void Ship::remove()
{
    Entity::remove();
    mShowExplosion = false;
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
    if (mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

void Aircraft::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    sf::Vector2f worldPosition = getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(
        [effect, worldPosition] (SoundNode& node, sf::Time)
        {
            node.playSound(effect, worldPosition);
        });

    commands.push(commands);
}

void Ship::updateMovementPattern(sf::Time dt)
{
    // Enemy airplane: Movement pattern
    const std::vector<Direction>& directions = Table[mType].directions;
    if (!directions.empty())
    {
        // Moved long enough in current direction: Change direction
        if (mTravelledDistance > directions[mDirectionIndex].distance)
        {
            mDirectionIndex = (mDirectionIndex + 1) % directions.size();
            mTravelledDistance = 0.f;
        }

        // Compute velocity from directions
        float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
        float vx = getMaxSpeed() * std::cos(radians);
        float vy = getMaxSpeed() * std::sin(radians);

        setVelocity(vx, vy);

        mTravelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Ship::checkPickupDrop(CommandQueue& commands)
{
    if (!isAllied() && randomInt(3) == 0 && !mSpawnedPickup)
        commands.push(mDropPickupCommand);

    mSpawnedPickup = true;
}

void Ship::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
    // Enemies try to fire all the time
    if (!isAllied())
        fire();

    // Check for automatic gunfire, allow only in intervals
    if (mIsFiring && mFireCountdown <= sf::Time::Zero)
    {
        // Interval expired: We can fire a new bullet
        commands.push(mFireCommand);
        playLocalSound(commands, isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);

        mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero)
    {
        // Interval not expired: Decrease it further
        mFireCountdown -= dt;
        mIsFiring = false;
    }

    if (mIsLaunchingMissile)
    {
        commands.push(mMissileCommand);
        playLocalSound(commands, SoundEffect::LaunchMissile);

        mIsLaunchingMissile = false;
    }
}

void Ship::createBullets(SceneNode& node, const TextureManager& textures) const
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

    switch (mSpreadLevel)
    {
        case 1:
            createProjectile(node, type, 0.0f, 0.5f, textures);
            break;

        case 2:
            createProjectile(node, type, -0.33f, 0.33f, textures);
            createProjectile(node, type, +0.33f, 0.33f, textures);
            break;

        case 3:
            createProjectile(node, type, -0.5f, 0.33f, textures);
            createProjectile(node, type,  0.0f,  0.5f, textures);
            createProjectile(node, type, +0.5f, 0.33f, textures);
            break;
    }
}

void Ship::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureManager& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

    sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
    sf::Vector2f velocity(0, projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : +1.f;
    projectile->setPosition(getWorldPosition() + offset * sign);
    projectile->setVelocity(velocity * sign);
    node.attachChild(std::move(projectile));
}

void Ship::createPickup(SceneNode& node, const TextureManager& textures) const
{
    auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

    std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
    pickup->setPosition(getWorldPosition());
    pickup->setVelocity(0.f, 1.f);
    node.attachChild(std::move(pickup));
}

void Ship::updateTexts()
{
    if (isDestroyed())
        mHealthDisplay->setString("");
    else
        mHealthDisplay->setString(toString(getHitpoints()) + " HP");
    mHealthDisplay->setPosition(0.f, 50.f);
    mHealthDisplay->setRotation(-getRotation());

    // Display missiles, if available
    if (mMissileDisplay)
    {
        if (mMissileAmmo == 0)
            mMissileDisplay->setString("");
        else
            mMissileDisplay->setString("M: " + toString(mMissileAmmo));
    }
}

void Ship::updateRollAnimation()
{
    if (Table[mType].hasRollAnimation)
    {
        sf::IntRect textureRect = Table[mType].textureRect;

        // Roll left: Texture rect offset once
        if (getVelocity().x < 0.f)
            textureRect.left += textureRect.width;

        // Roll right: Texture rect offset twice
        else if (getVelocity().x > 0.f)
            textureRect.left += 2 * textureRect.width;

        mSprite.setTextureRect(textureRect);
    }
}
