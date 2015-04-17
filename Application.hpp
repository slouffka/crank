#ifndef CRANK_APPLICATION_HPP
#define CRANK_APPLICATION_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Application
{
    public:
                                Application();
        void                    run();


    private:
        void                    processInput();
        void                    update(sf::Time frameTime);
        void                    render();

        void                    updateStatistics(sf::Time frameTime);
        void                    registerStates();


    private:
        static const sf::Time   TimePerFrame;

        sf::RenderWindow        mWindow;
        TextureManager          mTextures;
        FontManager             mFonts;
        Player                  mPlayer;

        MusicPlayer             mMusic;
        SoundPlayer             mSounds;
        StateStack              mStateStack;

        sf::Text                mStatisticsText;
        sf::Time                mStatisticsUpdateTime;
        std::size_t             mStatisticsNumFrames;
};

#endif // CRANK_APPLICATION_HPP
