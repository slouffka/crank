#ifndef CRANK_GAME_HPP
#define CRANK_GAME_HPP

#include "World.hpp"
#include "Player.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class Game : private sf::NonCopyable
{
    public:
                                Game();
        void                    run();


    private:
        void                    processInput();
        void                    update(sf::Time frameTime);
        void                    render();
        void                    updateStatistics(sf::Time frameTime);


    private:
        static const sf::Time   TimePerFrame;

        sf::RenderWindow        mWindow;
        World                   mWorld;
        Player                  mPlayer;

        FontManager             mFonts;
        sf::Text                mStatisticsText;
        sf::Time                mStatisticsUpdateTime;
        std::size_t             mStatisticsNumFrames;
};

#endif // CRANK_GAME_HPP
