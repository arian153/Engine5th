#pragma once
#include "Constraint.hpp"
#include "../../Utility/ConstraintUtility.hpp"

namespace Engine5
{
    class RigidBody;

    class PointConstraint final : public Constraint
    {
    public:
        explicit PointConstraint(RigidBody* body, ConstraintUtility* utility);
        ~PointConstraint();

        void GenerateVelocityConstraints(Real dt) override;
        void GeneratePositionConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;
        void ApplyPositionConstraints() override;

        void SetConstraintMode(eConstraintMode mode);
        void SetFrequency(Real frequency);
        void SetDampingRatio(Real damping_ratio);
        void EnableRotation(bool b_rotation);

    private:
        ConstraintUtility* m_constraint_utility = nullptr;
        RigidBody*         m_body               = nullptr;

        eConstraintMode m_mode = eConstraintMode::Hard;
        ConstraintBias  m_bias;

        Real m_frequency     = 5.0f;
        Real m_damping_ratio = 0.5f;
        bool m_b_rotation    = true;

        Vector3  m_total_lambda;
        Vector3  m_r;
        Matrix33 m_effective_mass;
        Matrix33 m_cross;
        Vector3  m_position_error_bias;
        Vector3  m_local_anchor;
        Vector3  m_target;

        Real     m_m = 1.0f;
        Matrix33 m_i;
        Vector3  m_v;
        Vector3  m_w;
    };
}
