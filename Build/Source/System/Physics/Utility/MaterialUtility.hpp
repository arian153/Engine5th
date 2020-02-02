#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class ColliderPrimitive;

    class MaterialUtility
    {
    public:
        static Real SolveRestitution(ColliderPrimitive* a, ColliderPrimitive* b);
        static Real SolveFriction(ColliderPrimitive* a, ColliderPrimitive* b);

    private:
    };

}
