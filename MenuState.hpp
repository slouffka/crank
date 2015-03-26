#ifndef CRANK_MENUSTATE_HPP
#define CRANK_MENUSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class MenuState : public State
{
    public:
                                MenuState(StateStack& stack, Context context);

        virtual void            draw();
        virtual bool            update(sf::Time frameTime);
        virtual bool            handleEvent(const sf::Event& event);


    private:
        sf::Sprite              mBackgroundSprite;
        GUI::Container          mGUIContainer;
};

#endif // CRANK_MENUSTATE_HPP
