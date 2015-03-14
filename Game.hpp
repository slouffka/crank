#ifndef CRANK_GAME_HPP
#define CRANK_GAME_HPP

#include "World.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class Game : private sf::NonCopyable
{
    public:
                                Game();
        void                    run();


    private:
        void                    processEvents();
        void                    update(sf::Time frameTime);
        void                    render();

        void                    updateStatistics(sf::Time frameTime);
        void                    handlePlayerInput(sf::Keyboard::Key key, bool isPressed);


    private:
        static const sf::Time   TimePerFrame;

        sf::RenderWindow        mWindow;
        World                   mWorld;

        FontManager             mFonts;
        sf::Text                mStatisticsText;
        sf::Time                mStatisticsUpdateTime;
        std::size_t             mStatisticsNumFrames;
};

#endif // CRANK_GAME_HPP
