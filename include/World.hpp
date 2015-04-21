#ifndef CRANK_WORLD_HPP
#define CRANK_WORLD_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Ship.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"
#include "Pickup.hpp"
#include "BloomEffect.hpp"
#include "SoundPlayer.hpp"
#include "NetworkProtocol.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
    class RenderTarget;
}

class NetworkNode;

class World : private sf::NonCopyable
{
    public:
        explicit                            World(sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, bool networked = false);
        void                                update(sf::Time dt);
        void                                draw();

        sf::FloatRect                       getViewBounds() const;
        CommandQueue&                       getCommandQueue();
        Ship*                               addShip(int identifier);
        void                                removeShip(int identifier);
        void                                setCurrentBattleFieldPosition(float lineY);
        void                                setWorldHeight(float height);

        void                                addEnemy(Ship::Type type, float relX, float relY);
        void                                sortEnemies();

        bool                                hasAlivePlayer() const;
        bool                                hasPlayerReachedEnd() const;

        void                                setWorldScrollCompensation(float compensation);

        Ship*                               getShip(int identifier) const;
        sf::FloatRect                       getBattlefieldBounds() const;

        void                                createPickup(sf::Vector2f position, Pickup::Type type);
        bool                                pollGameAction(GameActions::Action& out);


    private:
        void                                loadTextures();
        void                                adaptPlayerPosition();
        void                                adaptPlayerVelocity();
        void                                handleCollisions();
        void                                updateSounds();

        void                                buildScene();
        void                                addEnemies();
        void                                spawnEnemies();
        void                                destroyEntitiesOutsideView();
        void                                guideMissiles();


    private:
        enum Layer
        {
            Background,
            LowerSpace,
            UpperSpace,
            LayerCount
        };

        struct SpawnPoint
        {
            SpawnPoint(Ship::Type type, float x, float y)
            : type(type)
            , x(x)
            , y(y)
            {
            }

            Ship::Type type;
            float x;
            float y;
        };


    private:
        sf::RenderTarget&                   mTarget;
        sf::RenderTexture                   mSceneTexture;
        sf::View                            mWorldView;
        TextureManager                      mTextures;
        FontManager&                        mFonts;
        SoundPlayer&                        mSounds;

        SceneNode                           mSceneGraph;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        CommandQueue                        mCommandQueue;

        sf::FloatRect                       mWorldBounds;
        sf::Vector2f                        mSpawnPosition;
        float                               mScrollSpeed;
        float                               mScrollSpeedCompensation;
        std::vector<Ship*>                  mPlayerShips;

        std::vector<SpawnPoint>             mEnemySpawnPoints;
        std::vector<Ship*>                  mActiveEnemies;

        BloomEffect                         mBloomEffect;

        bool                                mNetworkedWorld;
        NetworkNode*                        mNetworkNode;
        SpriteNode*                         mFinishSprite;
};

#endif // CRANK_WORLD_HPP
