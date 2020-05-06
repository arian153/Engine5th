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

    private:
        Vector3 position;
        Vector3 velocity;
        Color   color;
        bool    b_active = false;
    };
}
