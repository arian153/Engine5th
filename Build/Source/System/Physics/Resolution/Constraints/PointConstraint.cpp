#include "PointConstraint.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"
#include "../../Utility/ConstraintUtility.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../../../Math/Algebra/Matrix33.hpp"

namespace Engine5
{
    PointConstraint::PointConstraint(RigidBody* body, ConstraintUtility* utility)
        : m_constraint_utility(utility), m_body(body)
    {
    }

    PointConstraint::~PointConstraint()
    {
    }

    void PointConstraint::GenerateVelocityConstraints(Real dt)
    {
        m_m           = m_body->InverseMass();
        m_i           = m_body->InverseInertia();
        m_v           = m_body->GetLinearVelocity();
        m_w           = m_body->GetAngularVelocity();
        m_r           = m_body->GetOrientation().Rotate(m_local_anchor - m_body->GetLocalCentroid());
        m_bias        = m_constraint_utility->GenerateConstraintBias(m_body->Mass(), m_frequency, m_damping_ratio, m_mode, dt);
        Vector3 c_pos = (m_body->GetCentroid() + m_r) - m_target;
        Vector3 c_vel = m_v + CrossProduct(m_w, m_r);
        m_cross.SetSkew(-m_r);
        Matrix33 k = m_m * Matrix33();
        if (m_b_rotation)
        {
            k += m_cross * m_i * m_cross.Transpose();
        }
        k += m_bias.softness_bias * Matrix33();
        m_position_error_bias = m_bias.position_bias * c_pos;
        m_effective_mass      = k.Inverse();
        m_total_lambda.SetZero();
    }

    void PointConstraint::SolveVelocityConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        Vector3 cVel   = m_v + CrossProduct(m_w, m_r);
        Vector3 jvb    = cVel + m_position_error_bias + m_bias.softness_bias * m_total_lambda;
        Vector3 lambda = m_effective_mass * (-jvb);
        m_total_lambda += lambda;
        m_v += m_m * lambda;
        if (m_b_rotation)
        {
            m_w += m_i * m_cross.Transpose() * lambda;
        }
    }

    void PointConstraint::ApplyVelocityConstraints()
    {
        m_body->SetLinearVelocity(m_v);
        m_body->SetAngularVelocity(m_w);
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

    void PointConstraint::SetConstraintMode(eConstraintMode mode)
    {
        m_mode = mode;
    }

    void PointConstraint::SetFrequency(Real frequency)
    {
        m_frequency = Math::Clamp(frequency, 0.001f, 10.0f);
    }

    void PointConstraint::SetDampingRatio(Real damping_ratio)
    {
        m_damping_ratio = Math::Clamp(damping_ratio, 0.0f, 10.0f);;
    }

    void PointConstraint::EnableRotation(bool b_rotation)
    {
        m_b_rotation = b_rotation;
    }
}
