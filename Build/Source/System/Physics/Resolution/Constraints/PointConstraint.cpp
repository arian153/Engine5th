#include "PointConstraint.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    PointConstraint::PointConstraint()
    {
    }

    PointConstraint::~PointConstraint()
    {
    }

    void PointConstraint::GenerateVelocityConstraints()
    {
    }

    void PointConstraint::GeneratePositionConstraints()
    {
    }

    void PointConstraint::SolveVelocityConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void PointConstraint::SolvePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }
}
