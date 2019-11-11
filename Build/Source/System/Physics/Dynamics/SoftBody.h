#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class SoftBody
    {
    public:
        SoftBody();
        ~SoftBody();

        private:
        friend class Resolution;
        friend class World;

    private:
    };
}
