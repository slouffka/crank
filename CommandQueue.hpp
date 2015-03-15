#ifndef CRANK_COMMANDQUEUE_HPP
#define CRANK_COMMANDQUEUE_HPP

#include "Command.hpp"

#include <queue>


class CommandQueue
{
    public:
        void                    push(const Command& command);
        Command                 pop();
        bool                    isEmpty() const;


    private:
        std::queue<Command>     mQueue;
};

#endif // CRANK_COMMANDQUEUE_HPP
