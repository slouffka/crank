clang++ -std=c++0x -O3 -lsfml-window -lsfml-graphics -lsfml-system -o crank \
    Utility.cpp         \
    ParallelTask.cpp    \
    Entity.cpp          \
    Ship.cpp            \
    Command.cpp         \
    CommandQueue.cpp    \
    Player.cpp          \
    SceneNode.cpp       \
    SpriteNode.cpp      \
    State.cpp           \
    StateStack.cpp      \
    TitleState.cpp      \
    LoadingState.cpp    \
    MenuState.cpp       \
    GameState.cpp       \
    PauseState.cpp      \
    World.cpp           \
    Application.cpp     \
    main.cpp
