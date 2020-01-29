#include "Resolution.hpp"
#include "ContactManifold.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/Constraints.hpp"

namespace Engine5
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::SolveConstraints(Constraints* constraints, Real dt) const
    {
        constraints->InitializeConstraints();
        constraints->SolveConstraints(dt);
        constraints->ApplyConstraints();
    }

}
