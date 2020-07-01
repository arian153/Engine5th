#pragma once
#include "Constraint.hpp"

namespace Engine5
{
    class RigidBody;

    class PointConstraint final : public Constraint
    {
    public:
        PointConstraint();
        ~PointConstraint();

        void GenerateVelocityConstraints(Real dt) override;
        void GeneratePositionConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;
        void ApplyPositionConstraints() override;
    private:
        RigidBody* m_body = nullptr;
    };
}
