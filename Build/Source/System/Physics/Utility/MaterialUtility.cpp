#include "MaterialUtility.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    Real MaterialUtility::SolveRestitution(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        return 1.0f;
    }

    Real MaterialUtility::SolveFriction(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        return 1.0f;
    }
}
