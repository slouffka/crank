#ifndef CRANK_TITLESTATE_HPP
#define CRANK_TITLESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState : public State
{
    public:
        TitleState(StateStack& stack, Context context);

        virtual void        draw();
        virtual bool        update(sf::Time frameTime);
        virtual bool        handleEvent(const sf::Event& event);


    private:
        sf::Sprite          mBackgroundSprite;
        sf::Text            mText;

        bool                mShowText;
        sf::Time            mTextEffectTime;
};

#endif // CRANK_TITLESTATE_HPP