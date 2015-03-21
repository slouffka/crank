#ifndef CRANK_PAUSESTATE_HPP
#define CRANK_PAUSESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState : public State
{
    public:
        PauseState(StateStack& stack, Context context);

        virtual void        draw();
        virtual bool        update(sf::Time frameTime);
        virtual bool        handleEvent(const sf::Event& event);


    private:
        sf::Sprite          mBackgroundSprite;
        sf::Text            mPausedText;
        sf::Text            mInstructionText;
};

#endif // CRANK_PAUSESTATE_HPP
