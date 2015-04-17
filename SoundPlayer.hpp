#ifndef CRANK_SOUNDPLAYER_HPP
#define CRANK_SOUNDPLAYER_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
    public:
                        SoundPlayer();

        void            play(SoundEffect::ID effect);
        void            play(SoundEffect::ID effect, sf::Vector2f position);

        void            removeStoppedSounds();
        void            setListenerPosition(sf::Vector2f position);
        sf::Vector2f    getListenerPosition() const;


    private:
        SoundBufferManager      mSoundBuffers;
        std::list<sf::Sound>    mSounds;
};

#endif // CRANK_SOUNDPLAYER_HPP
