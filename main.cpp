#include <SFML/Graphics.hpp>

class Game
{
    public:
                            Game();
        void                run();

    private:
        void                processEvents();
        void                update(sf::Time frameTime);
        void                render();

        void                handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    private:
        static const float  PlayerSpeed;

        sf::RenderWindow    mWindow;
        sf::CircleShape     mPlayer;

        bool                mIsMovingUp;
        bool                mIsMovingDown;
        bool                mIsMovingLeft;
        bool                mIsMovingRight;
};

const float Game::PlayerSpeed = 250.f;

Game::Game()
: mWindow(sf::VideoMode(640, 480), "Crank Game", sf::Style::Close)
, mPlayer()
, mIsMovingUp(false)
, mIsMovingDown(false)
, mIsMovingLeft(false)
, mIsMovingRight(false)
{
    mPlayer.setRadius(40.f);
    mPlayer.setPosition(100.f, 100.f);
    mPlayer.setFillColor(sf::Color::Cyan);
}

void Game::run()
{
    sf::Clock clock;

    while (mWindow.isOpen())
    {
        sf::Time frameTime = clock.restart();
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
    if (key == sf::Keyboard::W)
        mIsMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if (key == sf::Keyboard::A)
        mIsMovingLeft = isPressed;
    else if (key == sf::Keyboard::D)
        mIsMovingRight = isPressed;
    else if (key == sf::Keyboard::Escape && isPressed)
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

int main()
{
    Game game;
    game.run();
}
