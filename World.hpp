#ifndef CRANK_WORLD_HPP
#define CRANK_WORLD_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Ship.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
    class RenderWindow;
}

class World : private sf::NonCopyable
{
    public:
        explicit                                World(sf::RenderWindow& window);
        void                                    update(sf::Time frameTime);
        void                                    draw();

        CommandQueue&                           getCommandQueue();


    private:
        void                                    loadTextures();
        void                                    buildScene();
        void                                    adaptPlayerPosition();
        void                                    adaptPlayerVelocity();


    private:
        enum Layer
        {
            Background,
            Space,
            LayerCount
        };


    private:
        sf::RenderWindow&                       mWindow;
        sf::View                                mWorldView;
        TextureManager                          mTextures;

        SceneNode                               mSceneGraph;
        std::array<SceneNode*, LayerCount>      mSceneLayers;
        CommandQueue                            mCommandQueue;

        sf::FloatRect                           mWorldBounds;
        sf::Vector2f                            mSpawnPosition;
        float                                   mScrollSpeed;
        Ship*                                   mPlayerShip;
};

#endif // CRANK_WORLD_HPP
