#ifndef CRANK_PARTICLE_HPP
#define CRANK_PARTICLE_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>


struct Particle
{
    enum Type
    {
        Propellant,
        Smoke,
        ParticleCount
    };

    sf::Vector2f    position;
    sf::Color       color;
    sf::Time        lifetime;
};

#endif // CRANK_PARTICLE_HPP
