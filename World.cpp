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

    // Move the player sidewards (plane scouts follow the main ship)
    sf::Vector2f position = mPlayerShip->getPosition();
    sf::Vector2f velocity = mPlayerShip->getVelocity();

    // If player touches borders, flip its X velocity
    if (position.x <= mWorldBounds.left + 150.f || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f)
    {
        velocity.x = -velocity.x;
        mPlayerShip->setVelocity(velocity);
    }

    // Apply movements
    mSceneGraph.update(frameTime);
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
    mTextures.load(Textures::Background, "res/img/background.png");
    mTextures.load(Textures::Ship, "res/img/ship.png");
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
    std::unique_ptr<Ship> leader(new Ship(Ship::Eagle, mTextures));
    mPlayerShip = leader.get();
    mPlayerShip->setPosition(mSpawnPosition);
    mPlayerShip->setVelocity(40.f, mScrollSpeed);
    mSceneLayers[Space]->attachChild(std::move(leader));

    // Add two escorting ships, placed relatively to main ship
    std::unique_ptr<Ship> leftEscort(new Ship(Ship::Raptor, mTextures));
    leftEscort->setPosition(-80.f, 50.f);
    mPlayerShip->attachChild(std::move(leftEscort));

    std::unique_ptr<Ship> rightEscort(new Ship(Ship::Raptor, mTextures));
    rightEscort->setPosition(80.f, 50.f);
    mPlayerShip->attachChild(std::move(rightEscort));
}
