#ifndef CRANK_GAME_HPP
#define CRANK_GAME_HPP

#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>

namespace Textures
{
    enum ID
    {
        Ship
    };
}

class Game
{
    public:
                                Game();
        void                    run();

    private:
        void                    processEvents();
        void                    update(sf::Time frameTime);
        void                    render();

        void                    handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    private:
        static const float      PlayerSpeed;
        static const sf::Time   TimePerFrame;

        ResourceManager<sf::Texture, Textures::ID> mTextures;

        sf::RenderWindow        mWindow;
        sf::Sprite              mPlayer;

        bool                    mIsMovingUp;
        bool                    mIsMovingDown;
        bool                    mIsMovingLeft;
        bool                    mIsMovingRight;
};

#endif // CRANK_GAME_HPP
