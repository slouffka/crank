#ifndef CRANK_LOADINGSTATE_HPP
#define CRANK_LOADINGSTATE_HPP

#include "State.hpp"
#include "ParallelTask.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class LoadingState : public State
{
    public:
                                LoadingState(StateStack& stack, Context context);

        virtual void            draw();
        virtual bool            update(sf::Time frameTime);
        virtual bool            handleEvent(const sf::Event& event);

        void                    setProgress(float percent);


    private:
        sf::Text                mLoadingText;
        sf::RectangleShape      mProgressBarBackground;
        sf::RectangleShape      mProgressBar;

        ParallelTask            mLoadingTask;
};

#endif // CRANK_LOADINGSTATE_HPP
