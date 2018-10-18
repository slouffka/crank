#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Ship.hpp"
#include "NetworkProtocol.hpp"

#include <SFML/Network/Packet.hpp>

#include <map>
#include <string>
#include <algorithm>


using namespace std::placeholders;

struct ShipMover
{
    ShipMover(float vx, float vy, int identifier)
    : velocity(vx, vy)
    , shipID(identifier)
    {
    }

    void operator() (Ship& ship, sf::Time) const
    {
        ship.accelerate(velocity * ship.getMaxSpeed());
    }

    sf::Vector2f velocity;
    int shipID;
};

struct ShipFireTrigger
{
    ShipFireTrigger(int identifier)
    : shipID(identifier)
    {
    }

    void operator() (Ship& ship, sf::Time) const
    {
        if (ship.getIdentifier() == shipID)
            ship.fire();
    }

    int shipID;
};

struct ShipMissileTrigger
{
    ShipMissileTrigger(int identifier)
    : shipID(identifier)
    {
    }

    void operator() (Ship& ship, sf::Time) const
    {
        if (ship.getIdentifier() == shipID)
            ship.launchMissile();
    }

    int shipID;
};


Player::Player(sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding)
: mKeyBinding(binding)
, mCurrentMissionStatus(MissionRunning)
, mIdentifier(identifier)
, mSocket(socket)
{
    // Set initial action bindings
    initializeActions();

    // Assign all categories to player's aircraft
    for (auto& pair : mActionBinding)
        pair.second.category = Category::PlayerShip;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    // Event
    if (event.type == sf::Event::KeyPressed)
    {
        Action action;
        if (mKeyBinding && mKeyBinding->checkAction(event.key.code, action) && !isRealtimeAction(action))
        {
            // Network connected -> send event over network
            if (mSocket)
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>(Client::PlayerEvent);
                packet << mIdentifier;
                packet << static_cast<sf::Int32>(action);
                mSocket->send(packet);
            }

            // Network disconnected -> local event
            else
            {
                commands.push(mActionBinding[action]);
            }
        }
    }

    // Realtime change (network connected)
    if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && mSocket)
    {
        Action action;
        if (mKeyBinding && mKeyBinding->checkAction(event.key.code, action) && isRealtimeAction(action))
        {
            // Send realtime change over network
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Client::PlayerRealtimeChange);
            packet << mIdentifier;
            packet << static_cast<sf::Int32>(action);
            packet << (event.type == sf::Event::KeyPressed);
            mSocket->send(packet);
        }
    }
}

bool Player::isLocal() const
{
    // No key binding means this player is remote
    return mKeyBinding != nullptr;
}

void Player::disableAllRealtimeActions()
{
    for (auto& action : mActionProxies)
    {
        sf::Packet packet;
        packet << static_cast<sf::Int32>(Client::PlayerRealtimeChange);
        packet << mIdentifier;
        packet << static_cast<sf::Int32>(action.first);
        packet << false;
        mSocket->send(packet);
    }
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
    // Check if this is a networked game and local player or just a single player game
    if ((mSocket && isLocal()) || !mSocket)
    {
        // Lookup all actions and push corresponding commands to queue
        std::vector<Action> activeActions = mKeyBinding->getRealtimeActions();
        for (Action action : activeActions)
            commands.push(mActionBinding[action]);
    }
}

void Player::handleRealtimeNetworkInput(CommandQueue& commands)
{
    if (mSocket && !isLocal())
    {
        // Traverse all realtime input proxies. Because this is a networked game, the input isn't handled directly
        for (auto pair : mActionProxies)
        {
            if (pair.second && isRealtimeAction(pair.first))
                commands.push(mActionBinding[pair.first]);
        }
    }
}

void Player::handleNetworkEvent(Action action, CommandQueue& commands)
{
    commands.push(mActionBinding[action]);
}

void Player::handleNetworkRealtimeChange(Action action, bool actionEnabled)
{
    mActionProxies[action] = actionEnabled;
}

void Player::setMissionStatus(MissionStatus status)
{
    mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
    return mCurrentMissionStatus;
}

void Player::initializeActions()
{
    mActionBinding[PlayerAction::MoveLeft].action         = derivedAction<Ship>(ShipMover(-1,  0, mIdentifier));
    mActionBinding[PlayerAction::MoveRight].action        = derivedAction<Ship>(ShipMover(+1,  0, mIdentifier));
    mActionBinding[PlayerAction::MoveUp].action           = derivedAction<Ship>(ShipMover( 0, -1, mIdentifier));
    mActionBinding[PlayerAction::MoveDown].action         = derivedAction<Ship>(ShipMover( 0, +1, mIdentifier));
    mActionBinding[PlayerAction::Fire].action             = derivedAction<Ship>(ShipFireTrigger(mIdentifier));
    mActionBinding[PlayerAction::LaunchMissile].action    = derivedAction<Ship>(ShipMissileTrigger(mIdentifier));
}
