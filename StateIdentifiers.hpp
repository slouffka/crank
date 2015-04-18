#ifndef CRANK_STATEIDENTIFIERS_HPP
#define CRANK_STATEIDENTIFIERS_HPP


namespace States
{
    enum ID
    {
        None,
        Title,
        Menu,
        Game,
        Loading,
        Pause,
        NetworkPause,
        Settings,
        GameOver,
        MissionSuccess,
        HostGame,
        JoinGame
    };
}

#endif // CRANK_STATEIDENTIFIERS_HPP
