#ifndef CRANK_SOUNDNODE_HPP
#define CRANK_SOUNDNODE_HPP


#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"


class SoundPlayer;

class SoundNode : public SceneNode
{
    public:
        explicit                SoundNode(SoundPlayer& player);
        void                    playSound(SoundEffect::ID sound, sf::Vector2f position);

        virtual unsigned int    getCategory() const;


    private:
        SoundPlayer&            mSounds;
};

#endif // CRANK_SOUNDNODE_HPP
