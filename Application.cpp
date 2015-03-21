#include "Application.hpp"
#include "Utility.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "TitleState.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"


const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
: mWindow(sf::VideoMode(800, 600), "Crank 05-States", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);

    mFonts.load(Fonts::Main, "res/fonts/arcade.ttf");
    mTextures.load(Textures::TitleScreen, "res/textures/title-screen.png");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(10.f, 10.f);
    mStatisticsText.setCharacterSize(14u);

    registerStates();
    mStateStack.pushState(States::Title);
}

void Application::run()
{
    // floating time step, most smooth rendering but can't
    // guarantee repeated results for the same scene
    /* sf::Clock clock;
    sf::Time frameTime = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        frameTime = clock.restart();

        processInput();
        update(frameTime);
        updateStatistics(frameTime);
        render();
    } */

    // fixed time step, jagged rendering but accurate physics
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        sf::Time frameTime = clock.restart();
        timeSinceLastUpdate += frameTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(frameTime);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time frameTime)
{
    mStateStack.update(frameTime);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);

    mWindow.display();
}

void Application::updateStatistics(sf::Time frameTime)
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

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
}