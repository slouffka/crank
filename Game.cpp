#include "Game.hpp"
#include "StringHelpers.hpp"

#include <SFML/Window/Event.hpp>


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(800, 600), "Crank Game", sf::Style::Close)
, mWorld(mWindow)
, mFonts()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    mFonts.load(Fonts::Arcade, "res/fonts/arcade.ttf");
    mStatisticsText.setFont(mFonts.get(Fonts::Arcade));
    mStatisticsText.setPosition(10.f, 10.f);
    mStatisticsText.setCharacterSize(14);
}

void Game::run()
{
    // floating time step, most smooth rendering but can't
    // guarantee repeated results for the same scene
    sf::Clock clock;
    sf::Time frameTime = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        frameTime = clock.restart();

        processEvents();
        update(frameTime);
        updateStatistics(frameTime);
        render();
    }

    // fixed time step, jagged rendering but accurate physics
    /* sf::Clock clock;
    sf::Time frameTime = sf::Time::Zero;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        frameTime = clock.restart();
        timeSinceLastUpdate += frameTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update(TimePerFrame);

        }

        updateStatistics(elapsedTime);
        render();
    } */
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

void Game::update(sf::Time frameTime)
{
    mWorld.update(frameTime);
}

void Game::render()
{
    mWindow.clear();
    mWorld.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::updateStatistics(sf::Time frameTime)
{
    mStatisticsUpdateTime += frameTime;
    mStatisticsNumFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString(
            toString(mStatisticsNumFrames) + " fps\n" +
            toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames / 1000.0f) + " ms/frame"
        );

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    // exit
    if (key == sf::Keyboard::Escape && isPressed)
        mWindow.close();
}
