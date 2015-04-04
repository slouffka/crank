#include "World.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Foreach.hpp"
#include "TextNode.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderWindow& window, FontManager& fonts)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mFonts(fonts)
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerShip(nullptr)
, mEnemySpawnPoints()
, mActiveEnemies()
{
    loadTextures();
    buildScene();

    // Prepare the view
    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time frameTime)
{
    // Scroll the world
    mWorldView.move(0.f, mScrollSpeed * frameTime.asSeconds());
    mPlayerShip->setVelocity(0.f, 0.f);

    // Setup commands to destroy entities, and guide missiles
    destroyEntitiesOutsideView();
    guideMissiles();

    // Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), frameTime);
    adaptPlayerVelocity();

    // Collision detection and response (may destroy entities)
    handleCollisions();

    // Remove all destroyed entities, create new ones
    mSceneGraph.removeWrecks();
    spawnEnemies();

    // Regular update step, adapt position (correct if outside view)
    mSceneGraph.update(frameTime, mCommandQueue);
    adaptPlayerPosition();
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
    return !mPlayerShip->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
    return !mWorldBounds.contains(mPlayerShip->getPosition());
}

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, "res/textures/eagle.png");
    mTextures.load(Textures::Raptor, "res/textures/raptor.png");
    mTextures.load(Textures::Avenger, "res/textures/avenger.png");
    mTextures.load(Textures::Background, "res/textures/background.png");
}

void World::buildScene()
{
    // Initialize the different layers
    for (std::size_t i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Prepare the tiled background
    sf::Texture& texture = mTextures.get(Textures::Background);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);

    // Add background sprite to the screen
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Add player's ship
    std::unique_ptr<Ship> player(new Ship(Ship::Eagle, mTextures));
    mPlayerShip = player.get();
    mPlayerShip->setPosition(mSpawnPosition);
    mSceneLayers[Space]->attachChild(std::move(player));
}

void World::adaptPlayerPosition()
{
    // Keep player's positioin inside the screen bounds, at least borderDistance units from border
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 40.f;

    sf::Vector2f position = mPlayerShip->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    mPlayerShip->setPosition(position);
}

void World::adaptPlayerVelocity()
{
    sf::Vector2f velocity = mPlayerShip->getVelocity();

    // If moving diagonally, reduce velocity (to have always same velocity)
    if (velocity.x != 0.f && velocity.y != 0.f)
        mPlayerShip->setVelocity(velocity / std::sqrt(2.f));

    // Add scrolling velocity
    mPlayerShip->accelerate(0.f, mScrollSpeed);
}
