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

    void PointConstraint::GenerateVelocityConstraints(Real dt)
    {
       
    }

    void PointConstraint::SolveVelocityConstraints(Real dt)
    {
      
    }

    void PointConstraint::ApplyVelocityConstraints()
    {
    }

    void PointConstraint::GeneratePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void PointConstraint::SolvePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void PointConstraint::ApplyPositionConstraints()
    {
    }
}
