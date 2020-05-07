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

    public:
        Vector3 position;
        Vector3 velocity;
        Color   color;
        Real    life  = 0.0f;
        Real    scale = 1.0f;
    };
}
