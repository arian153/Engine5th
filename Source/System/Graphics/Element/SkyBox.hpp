#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class SkyBox
    {
    public:
        SkyBox();
        ~SkyBox();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:

    };
}
