#pragma once
#include "../../Math/Algebra/Vector3.hpp"

namespace Engine5
{
    class Particle
    {
    public:
        Particle();
        ~Particle();

    public:
        Vector3 pos;
    };

    class ParticleEmitter
    {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

    private:
    };
}
