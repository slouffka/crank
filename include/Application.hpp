#ifndef CRANK_APPLICATION_HPP
#define CRANK_APPLICATION_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"
#include "KeyBinding.hpp"
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
        void                    update(sf::Time dt);
        void                    render();

        void                    updateStatistics(sf::Time dt);
        void                    registerStates();


    private:
        static const sf::Time   TimePerFrame;

        sf::RenderWindow        mWindow;
        TextureManager          mTextures;
        FontManager             mFonts;
        MusicPlayer             mMusic;
        SoundPlayer             mSounds;

        KeyBinding              mKeyBinding1;
        KeyBinding              mKeyBinding2;
        StateStack              mStateStack;

        sf::Text                mStatisticsText;
        sf::Time                mStatisticsUpdateTime;
        std::size_t             mStatisticsNumFrames;
};

#endif // CRANK_APPLICATION_HPP
