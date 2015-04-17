#ifndef CRANK_RESOURCEIDENTIFIERS_HPP
#define CRANK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
    class Texture;
    class Font;
    class Shader;
}

namespace Textures
{
    enum ID
    {
        Entities,
        Background,
        TitleScreen,
        Buttons,
        Explosion,
        Particle,
        FinishLine
    };
}

namespace Shaders
{
    enum ID
    {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass
    };
}

namespace Fonts
{
    enum ID
    {
        Main
    };
}

namespace SoundEffect
{
    enum ID
    {
        AlliedGunfire,
        EnemyGunfire,
        Explosion1,
        Explosion2,
        LaunchMissile,
        CollectPickup,
        Button
    };
}

namespace Music
{
    enum ID
    {
        MenuTheme,
        MissionTheme
    };
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceManager;

typedef ResourceManager<sf::Texture, Textures::ID>          TextureManager;
typedef ResourceManager<sf::Font, Fonts::ID>                FontManager;
typedef ResourceManager<sf::Shader, Shaders::ID>            ShaderManager;
typedef ResourceManager<sf::SoundBuffer, SoundEffect::ID>   SoundManager;

#endif // CRANK_RESOURCEIDENTIFIERS_HPP
