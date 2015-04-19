#ifndef CRANK_GAMEOVERSTATE_HPP
#define CRANK_GAMEOVERSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState : public State
{
    public:
        GameOverState(StateStack& stack, Context context, const std::string& text);

        virtual void        draw();
        virtual bool        update(sf::Time frameTime);
        virtual bool        handleEvent(const sf::Event& event);


    private:
        sf::Text            mGameOverText;
        sf::Time            mElapsedTime;
};

#endif // CRANK_GAMEOVERSTATE_HPP
