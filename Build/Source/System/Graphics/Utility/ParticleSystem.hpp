#pragma once
#include <vector>

namespace Engine5
{
    class ParticleEmitter;

    class ParticleSystem
    {
    public:
        ParticleSystem();
        ~ParticleSystem();

    private:
        std::vector<ParticleEmitter*> m_emitters;
    };
}
