#pragma once
#include "Constraint.hpp"

namespace Engine5
{
    class PointConstraint final : public Constraint
    {
    public:
        PointConstraint();
        ~PointConstraint();

        void Initialize() override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void Apply() override;
    private:
    };
}
