#pragma once
#include "Color.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace Engine5
{
    class Particle
    {
    public:
        Particle()
        {
        }

        ~Particle()
        {
        }

        bool IsActive() const
        {
            return life > 0.0f && scale > 0.0f;
        }

        Particle& Merge(const Particle& particle)
        {
            position += particle.position;
            velocity += particle.velocity;
            color.r += particle.color.r;
            color.g += particle.color.g;
            color.b += particle.color.b;
            color.a += particle.color.a;
            life += particle.life;
            scale += particle.scale;
            return *this;
        }

        Particle& operator=(const Particle& rhs)
        {
            if (&rhs != this)
            {
                position = rhs.position;
                velocity = rhs.velocity;
                color    = rhs.color;
                life     = rhs.life;
                scale    = rhs.scale;
            }
            return *this;
        }

    public:
        Vector3 position;
        Vector3 velocity;
        Color   color;
        Real    life  = 0.0f;
        Real    scale = 1.0f;
    };
}
