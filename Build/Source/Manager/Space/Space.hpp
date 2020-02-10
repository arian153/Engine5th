#pragma once
#include "../../System/Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Space
    {
    public:
        Space();
        ~Space();

        void Initialize();
        void Update(Real dt);
        void Shutdown();


    private:
        bool m_b_activate = false;
    };
}
