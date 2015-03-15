#include "Game.hpp"
#include "StringHelpers.hpp"

#include <SFML/Window/Event.hpp>


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(800, 600), "---=#_$ ^ *  crank ` . $_#=---", sf::Style::Close)
, mWorld(mWindow)
, mPlayer()
, mFonts()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);

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

        processInput();
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

            processInput();
            update(TimePerFrame);

        }

        updateStatistics(frameTime);
        render();
    } */
}

void Game::processInput()
{
    CommandQueue& commands = mWorld.getCommandQueue();

    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mPlayer.handleEvent(event, commands);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }

    mPlayer.handleRealtimeInput(commands);
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
