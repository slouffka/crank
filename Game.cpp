#include "Game.hpp"
#include "ResourceManager.hpp"

const float Game::PlayerSpeed = 400.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(800, 600), "Crank Game", sf::Style::Close)
, mTextures()
, mPlayer()
, mIsMovingUp(false)
, mIsMovingDown(false)
, mIsMovingLeft(false)
, mIsMovingRight(false)
{
    try
    {
        mTextures.load(Textures::Ship, "assets/img/ship.png");
    }
    catch (std::runtime_error& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        return;
    }

    mPlayer.setTexture(mTextures.get(Textures::Ship));
    mPlayer.setPosition(100.f, 100.f);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time frameTime = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        frameTime = clock.restart();

        processEvents();
        update(frameTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;

    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
            default:
                break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    // movement
    if (key == sf::Keyboard::E)
        mIsMovingUp = isPressed;
    if (key == sf::Keyboard::D)
        mIsMovingDown = isPressed;
    if (key == sf::Keyboard::S)
        mIsMovingLeft = isPressed;
    if (key == sf::Keyboard::F)
        mIsMovingRight = isPressed;

    // exit
    if (key == sf::Keyboard::Escape && isPressed)
        mWindow.close();
}

void Game::update(sf::Time frameTime)
{
    sf::Vector2f movement(0.f, 0.f);

    if (mIsMovingUp)
        movement.y -= PlayerSpeed;
    if (mIsMovingDown)
        movement.y += PlayerSpeed;
    if (mIsMovingLeft)
        movement.x -= PlayerSpeed;
    if (mIsMovingRight)
        movement.x += PlayerSpeed;

    mPlayer.move(movement * frameTime.asSeconds());
}

void Game::render()
{
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}
