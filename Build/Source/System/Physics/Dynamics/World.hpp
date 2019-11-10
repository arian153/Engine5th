#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class World
    {
    public:
        World();
        ~World();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:

    };
}
