#include "GameServer.hpp"
#include "NetworkProtocol.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "Ship.hpp"

#include <SFML/Network/Packet.hpp>

GameServer::RemotePeer::RemotePeer()
: ready(false)
, timedOut(false)
{
    socket.setBlocking(false);
}

GameServer::GameServer(sf::Vector2f battlefieldSize)
: mThread(&GameServer::executionThread, this)
, mListeningState(false)
, mClientTimeoutTime(sf::seconds(3.f))
, mMaxConnectedPlayers(10)
, mConnectedPlayers(0)
, mWorldHeight(5000.f)
, mBattleFieldRect(0.f, mWorldHeight - battlefieldSize.y, battlefieldSize.x, battlefieldSize.y)
, mBattleFieldScrollSpeed(-50.f)
, mShipCount(0)
, mPeers(1)
, mShipIdentifierCounter(1)
, mWaitingThreadEnd(false)
, mLastSpawnTime(sf::Time::Zero)
, mTimeForNextSpawn(sf::seconds(5.f))
{
    mListenerSocket.setBlocking(false);
    mPeers[0].reset(new RemotePeer());
    mThread.launch();
}

GameServer::~GameServer()
{
    mWaitingThreadEnd = true;
    mThread.wait();
}

void GameServer::notifyPlayerRealtimeChange(sf::Int32 shipIdentifier, sf::Int32 action, bool actionEnabled)
{
    for (std::size_t i = 0; i < mConnectedPlayers; ++i)
    {
        if (mPeers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Server::PlayerRealtimeChange);
            packet << shipIdentifier;
            packet << action;
            packet << actionEnabled;

            mPeers[i]->socket.send(packet);
        }
    }
}

void GameServer::notifyPlayerEvent(sf::Int32 shipIdentifier, sf::Int32 action)
{
    for (std::size_t i = 0; i < mConnectedPlayers; ++i)
    {
        if (mPeers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Server::PlayerEvent);
            packet << shipIdentifier;
            packet << action;

            mPeers[i]->socket.send(packet);
        }
    }
}

void GameServer::notifyPlayerSpawn(sf::Int32 shipIdentifier)
{
    for (std::size_t i = 0; i < mConnectedPlayers; ++i)
    {
        if (mPeers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Server::PlayerConnect);
            packet << shipIdentifier << mShipInfo[shipIdentifier].position.x << mShipInfo[shipIdentifier].position.y;
            mPeers[i]->socket.send(packet);
        }
    }
}

void GameServer::setListening(bool enable)
{
    // Check if it isn't already listening
    if (enable)
    {
        if (!mListeningState)
            mListeningState = (mListenerSocket.listen(ServerPort) == sf::TcpListener::Done);
    }
    else
    {
        mListenerSocket.close();
        mListeningState = false;
    }
}

void GameServer::executionThread()
{
    setListening(true);

    sf::Time stepInterval = sf::seconds(1.f / 60.f);
    sf::Time stepTime = sf::Time::Zero;
    sf::Time tickInterval = sf::seconds(1.f / 20.f);
    sf::Time tickTime = sf::Time::Zero;
    sf::Clock stepClock, tickClock;

    while (!mWaitingThreadEnd)
    {
        handleIncomingPackets();
        handleIncomingConnections();

        stepTime += stepClock.getElapsedTime();
        stepClock.restart();

        tickTime += tickClock.getElapsedTime();
        tickClock.restart();

        // Fixed update step
        while (stepTime >= stepInterval)
        {
            mBattleFieldRect.top += mBattleFieldScrollSpeed * stepInterval.asSeconds();
            stepTime -= stepInterval;
        }

        while (tickTime >= tickInterval)
        {
            tick();
            tickTime -= tickInterval;
        }

        // Sleep to prevent server from consuming 100% CPU
        sf::sleep(sf::milliseconds(100));
    }
}

void GameServer::tick()
{
    updateClientState();

    // Check for mission success = all ships with position.y < offset
    bool allShipsDone = true;
    for (auto pair : mShipInfo)
    {
        // As long as noe player has not crossed the finish line yet, set variable to false
        if (pair.second.position.y > 0.f)
            allShipsDone = false;
    }

    if (allShipsDone)
    {
        sf::Packet missionSuccessPacket;
        missionSuccessPacket << static_cast<sf::Int32>(Server::MissionSuccess);
        sendToAll(missionSuccessPacket);
    }

    // Remove IDs of ship that have been destroyed (relevant if a client has two, and loses one)
    for (auto itr = mShipInfo.begin(); itr != mShipInfo.end(); )
    {
        if (itr->second.hitpoints <= 0)
            mShipInfo.erase(itr++);
        else
            ++itr;
    }

    // Check if its time to attempt to spawn enemies
    if (now() >= mTimeForNextSpawn + mLastSpawnTime)
    {
        // No more enemies are spawned near the end
        if (mBattleFieldRect.top > 600.f)
        {
            std::size_t enemyCount = 1u + randomInt(2);
            float spawnCenter = static_cast<float>(randomInt(500) - 250);

            // In case only one enemy
            float shipDistance = 0.f;
            float nextSpawnPosition = spawnCenter;

            // In case there are two enemies being spawned together, each is spawned at each side of the spawnCenter, with a minimum distance
            if (enemyCount == 2)
            {
                shipDistance = static_cast<float>(150 + randomInt(250));
                nextSpawnPosition = spawnCenter - shipDistance / 2.f;
            }

            // Send the spawn orders to all clients
            for (std::size_t i = 0; i < enemyCount; ++i)
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>(Server::SpawnEnemy);
                packet << static_cast<sf::Int32>(1 + randomInt(Ship::TypeCount - 1));
                packet << mWorldHeight - mBattleFieldRect.top + 500;
                packet << nextSpawnPosition;

                nextSpawnPosition += shipDistance / 2.f;

                sendToAll(packet);
            }

            mLastSpawnTime = now();
            mTimeForNextSpawn = sf::milliseconds(2000 + randomInt(6000));
        }
    }
}

sf::Time GameServer::now() const
{
    return mClock.getElapsedTime();
}

void GameServer::handleIncomingPackets()
{
    bool detectedTimeout = false;

    for (PeerPtr& peer : mPeers)
    {
        if (peer->ready)
        {
            sf::Packet packet;
            while (peer->socket.receive(packet) == sf::Socket::Done)
            {
                // Interpret packet and react to it
                handleIncomingPacket(packet, *peer, detectedTimeout);

                // Packet was indeed received, update the ping timer
                peer->lastPacketTime = now();
                packet.clear();
            }

            if (now() >= peer->lastPacketTime + mClientTimeoutTime)
            {
                peer->timedOut = true;
                detectedTimeout = true;
            }
        }
    }

    if (detectedTimeout)
        handleDisconnections();
}

void GameServer::handleIncomingPacket(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout)
{
    sf::Int32 packetType;
    packet >> packetType;

    switch (packetType)
    {
        case Client::Quit:
        {
            receivingPeer.timedOut = true;
            detectedTimeout = true;
        } break;

        case Client::PlayerEvent:
        {
            sf::Int32 shipIdentifier;
            sf::Int32 action;
            packet >> shipIdentifier >> action;

            notifyPlayerEvent(shipIdentifier, action);
        } break;

        case Client::PlayerRealtimeChange:
        {
            sf::Int32 shipIdentifier;
            sf::Int32 action;
            bool actionEnabled;
            packet >> shipIdentifier >> action >> actionEnabled;
            mShipInfo[shipIdentifier].realtimeActions[action] = actionEnabled;
            notifyPlayerRealtimeChange(shipIdentifier, action, actionEnabled);
        } break;

        case Client::RequestCoopPartner:
        {
            receivingPeer.shipIdentifiers.push_back(mShipIdentifierCounter);
            mShipInfo[mShipIdentifierCounter].position = sf::Vector2f(mBattleFieldRect.width / 2, mBattleFieldRect.top + mBattleFieldRect.height / 2);
            mShipInfo[mShipIdentifierCounter].hitpoints = 100;
            mShipInfo[mShipIdentifierCounter].missileAmmo = 2;

            sf::Packet requestPacket;
            requestPacket << static_cast<sf::Int32>(Server::AcceptCoopPartner);
            requestPacket << mShipIdentifierCounter;
            requestPacket << mShipInfo[mShipIdentifierCounter].position.x;
            requestPacket << mShipInfo[mShipIdentifierCounter].position.y;

            receivingPeer.socket.send(requestPacket);
            mShipCount++;

            // Inform every other peer about this new ship
            for (PeerPtr& peer : mPeers)
            {
                if (peer.get() != &receivingPeer && peer->ready)
                {
                    sf::Packet notifyPacket;
                    notifyPacket << static_cast<sf::Int32>(Server::PlayerConnect);
                    notifyPacket << mShipIdentifierCounter;
                    notifyPacket << mShipInfo[mShipIdentifierCounter].position.x;
                    notifyPacket << mShipInfo[mShipIdentifierCounter].position.y;
                    peer->socket.send(notifyPacket);
                }
            }
            mShipIdentifierCounter++;
        } break;

        case Client::PositionUpdate:
        {
            sf::Int32 numShips;
            packet >> numShips;

            for (sf::Int32 i = 0; i < numShips; ++i)
            {
                sf::Int32 shipIdentifier;
                sf::Int32 shipHitpoints;
                sf::Int32 missileAmmo;
                sf::Vector2f shipPosition;
                packet >> shipIdentifier >> shipPosition.x >> shipPosition.y >> shipHitpoints >> missileAmmo;
                mShipInfo[shipIdentifier].position = shipPosition;
                mShipInfo[shipIdentifier].hitpoints = shipHitpoints;
                mShipInfo[shipIdentifier].missileAmmo = missileAmmo;
            }
        } break;

        case Client::GameEvent:
        {
            sf::Int32 action;
            float x;
            float y;

            packet >> action;
            packet >> x;
            packet >> y;

            // Enemy explodes: With certain probability, drop pickup
            // To avoid multiple messages spawning multiple pickups, only listen to first peer (host)
            if (action == GameActions::EnemyExplode && randomInt(3) == 0 && &receivingPeer == mPeers[0].get())
            {
                sf::Packet packet;
                packet << static_cast<sf::Int32>(Server::SpawnPickup);
                packet << static_cast<sf::Int32>(randomInt(Pickup::TypeCount));
                packet << x;
                packet << y;

                sendToAll(packet);
            }
        }
    }
}

void GameServer::updateClientState()
{
    sf::Packet updateClientStatePacket;
    updateClientStatePacket << static_cast<sf::Int32>(Server::UpdateClientState);
    updateClientStatePacket << static_cast<float>(mBattleFieldRect.top + mBattleFieldRect.height);
    updateClientStatePacket << static_cast<sf::Int32>(mShipInfo.size());

    for (auto ship : mShipInfo)
        updateClientStatePacket << ship.first << ship.second.position.x << ship.second.position.y;

    sendToAll(updateClientStatePacket);
}

void GameServer::handleIncomingConnections()
{
    if (!mListeningState)
        return;

    if (mListenerSocket.accept(mPeers[mConnectedPlayers]->socket) == sf::TcpListener::Done)
    {
        // order the new client to spawn its own ship ( player 1)
        mShipInfo[mShipIdentifierCounter].position = sf::Vector2f(mBattleFieldRect.width / 2, mBattleFieldRect.top + mBattleFieldRect.height / 2);
        mShipInfo[mShipIdentifierCounter].hitpoints = 100;
        mShipInfo[mShipIdentifierCounter].missileAmmo = 2;

        sf::Packet packet;
        packet << static_cast<sf::Int32>(Server::SpawnSelf);
        packet << mShipIdentifierCounter;
        packet << mShipInfo[mShipIdentifierCounter].position.x;
        packet << mShipInfo[mShipIdentifierCounter].position.y;

        mPeers[mConnectedPlayers]->shipIdentifiers.push_back(mShipIdentifierCounter);

        broadcastMessage("New player!");
        informWorldState(mPeers[mConnectedPlayers]->socket);
        notifyPlayerSpawn(mShipIdentifierCounter++);

        mPeers[mConnectedPlayers]->socket.send(packet);
        mPeers[mConnectedPlayers]->ready = true;
        mPeers[mConnectedPlayers]->lastPacketTime = now(); // prevent initial timeouts
        mShipCount++;
        mConnectedPlayers++;

        if (mConnectedPlayers >= mMaxConnectedPlayers)
            setListening(false);
        else // Add a new waiting peer
            mPeers.push_back(PeerPtr(new RemotePeer()));
    }
}

void GameServer::handleDisconnections()
{
    for (auto itr = mPeers.begin(); itr != mPeers.end(); )
    {
        if ((*itr)->timedOut)
        {
            // Inform everyone of the disconnection, erase
            for (sf::Int32 identifier : (*itr)->shipIdentifiers)
            {
                sendToAll(sf::Packet() << static_cast<sf::Int32>(Server::PlayerDisconnect) << identifier);

                mShipInfo.erase(identifier);
            }

            mConnectedPlayers--;
            mShipCount -= (*itr)->shipIdentifiers.size();

            itr = mPeers.erase(itr);

            // Go back to a listening state if needed
            if (mConnectedPlayers < mMaxConnectedPlayers)
            {
                mPeers.push_back(PeerPtr(new RemotePeer()));
                setListening(true);
            }

            broadcastMessage("An ally has disconnected.");
        }
        else
        {
            ++itr;
        }
    }
}

// Tell the newly connected peer about how the world is currently
void GameServer::informWorldState(sf::TcpSocket& socket)
{
    sf::Packet packet;
    packet << static_cast<sf::Int32>(Server::InitialState);
    packet << mWorldHeight << mBattleFieldRect.top + mBattleFieldRect.height;
    packet << static_cast<sf::Int32>(mShipCount);

    for (std::size_t i = 0; i < mConnectedPlayers; ++i)
    {
        if (mPeers[i]->ready)
        {
            for (sf::Int32 identifier : mPeers[i]->shipIdentifiers)
                packet << identifier << mShipInfo[identifier].position.x << mShipInfo[identifier].position.y << mShipInfo[identifier].hitpoints << mShipInfo[identifier].missileAmmo;
        }
    }

    socket.send(packet);
}

void GameServer::broadcastMessage(const std::string& message)
{
    for (std::size_t i = 0; i < mConnectedPlayers; ++i)
    {
        if (mPeers[i]->ready)
        {
            sf::Packet packet;
            packet << static_cast<sf::Int32>(Server::BroadcastMessage);
            packet << message;

            mPeers[i]->socket.send(packet);
        }
    }
}

void GameServer::sendToAll(sf::Packet& packet)
{
    for (PeerPtr& peer : mPeers)
    {
        if (peer->ready)
            peer->socket.send(packet);
    }
}
