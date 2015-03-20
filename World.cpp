#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerShip(nullptr)
, mEnemyShip(nullptr)
{
    loadTextures();
    buildScene();

    // Prepare the view
    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time frameTime)
{
    // Scroll the world
    // mWorldView.move(0.f, mScrollSpeed * frameTime.asSeconds());
    mPlayerShip->setVelocity(0.f, 0.f);

    // Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), frameTime);
    adaptPlayerVelocity();

    mSceneGraph.update(frameTime);
    adaptPlayerPosition();

    // mEnemyShip->setPosition(mPlayerShip->getPosition().x - 200.f, mPlayerShip->getPosition().y - 200.f);
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

void World::loadTextures()
{
    mTextures.load(Textures::Background, "res/img/background.png");
    mTextures.load(Textures::Eagle, "res/img/eagle.png");
    mTextures.load(Textures::Raptor, "res/img/raptor.png");
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

    // Add enemy ship
    std::unique_ptr<Ship> enemy(new Ship(Ship::Raptor, mTextures));
    mEnemyShip = enemy.get();
    mEnemyShip->setPosition(mSpawnPosition.x + 200.f, mSpawnPosition.y + 200.f);
    mSceneLayers[Space]->attachChild(std::move(enemy));
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
