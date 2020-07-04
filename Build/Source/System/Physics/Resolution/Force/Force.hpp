#pragma once
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class RigidBody;

    class Force
    {
    public:
        Force()
        {
        }

        virtual ~Force()
        {
        }

        virtual void Update(RigidBody* body, Real dt) = 0;
    protected:
    };
}
