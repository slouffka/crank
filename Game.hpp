#ifndef CRANK_GAME_HPP
#define CRANK_GAME_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "StringHelpers.hpp"
#include "ResourceManager.hpp"


namespace Textures
{
    enum ID
    {
        Ship
    };
}

namespace Fonts
{
    enum ID
    {
        Arcade
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
        void                    updateStats(sf::Time frameTime);

    private:
        static const float      PlayerSpeed;
        static const sf::Time   TimePerFrame;

        ResourceManager<sf::Texture, Textures::ID> mTextures;
        ResourceManager<sf::Font, Fonts::ID> mFonts;

        sf::RenderWindow        mWindow;
        sf::Sprite              mPlayer;

        sf::Text                mStatsText;
        sf::Time                mStatsUpdateTime;
        std::size_t             mStatsNumFrames;

        bool                    mIsMovingUp;
        bool                    mIsMovingDown;
        bool                    mIsMovingLeft;
        bool                    mIsMovingRight;
};

#endif // CRANK_GAME_HPP
