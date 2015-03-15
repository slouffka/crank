#ifndef CRANK_COMMAND_HPP
#define CRANK_COMMAND_HPP

#include "Category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>


class SceneNode;

struct Command
{
                                                    Command();

    std::function<void(SceneNode&, sf::Time)>       action;
    unsigned int                                    category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
    return [=] (SceneNode& node, sf::Time frameTime)
    {
        // Check if cast is safe
        assert(dynamic_cast<GameObject*>(&node) != nullptr);

        // Downcast node and invoke function on it
        fn(static_cast<GameObject&>(node), frameTime);
    };
}

#endif // CRANK_COMMAND_HPP
