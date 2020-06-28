#pragma once
#include "Constraint.hpp"

namespace Engine5
{
    class PointConstraint final : public Constraint
    {
    public:
        PointConstraint();
        ~PointConstraint();

        void GenerateVelocityConstraints(Real dt) override;
        void GeneratePositionConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
    private:
    };
}
