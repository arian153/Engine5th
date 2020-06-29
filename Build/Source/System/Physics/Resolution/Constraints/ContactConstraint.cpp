#include "ContactConstraint.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../ContactManifold.hpp"
#include "../ContactPoint.hpp"
#include "../../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"
#include "../../../Graphics/DataType/Color.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    ContactConstraint::ContactConstraint(ContactManifold* input, Physics::FrictionUtility* friction_utility, Real tangent_speed)
        : m_friction_utility(friction_utility), m_manifold(input), m_tangent_speed(tangent_speed)
    {
    }

    ContactConstraint::~ContactConstraint()
    {
    }

    void ContactConstraint::GenerateVelocityConstraints(Real dt)
    {
        m_body_a = m_manifold->m_set_a->GetRigidBody();
        m_body_b = m_manifold->m_set_b->GetRigidBody();
        //set mass
        m_mass.m_a = m_body_a->Mass();
        m_mass.i_a = m_body_a->Inertia();
        m_mass.m_b = m_body_b->Mass();
        m_mass.i_b = m_body_b->Inertia();
        //velocity term
        m_velocity.v_a = m_body_a->GetLinearVelocity();
        m_velocity.w_a = m_body_a->GetAngularVelocity();
        m_velocity.v_b = m_body_b->GetLinearVelocity();
        m_velocity.w_b = m_body_b->GetAngularVelocity();
        m_count        = m_manifold->contacts.size();
        Basis basis;
        for (size_t i = 0; i < m_count; ++i)
        {
            basis.CalculateBasisApprox(m_manifold->contacts[i].normal);
            m_manifold->contacts[i].r_a = m_manifold->contacts[i].global_position_a - m_body_a->GetCentroid();
            m_manifold->contacts[i].r_b = m_manifold->contacts[i].global_position_b - m_body_b->GetCentroid();
            InitializeJacobianVelocity(m_manifold->contacts[i], basis.i, m_normal[i], dt, true);
            InitializeJacobianVelocity(m_manifold->contacts[i], basis.j, m_tangent[i], dt);
            InitializeJacobianVelocity(m_manifold->contacts[i], basis.k, m_bitangent[i], dt);
        }
    }

    void ContactConstraint::GeneratePositionConstraints(Real dt)
    {
        if (m_body_a != nullptr)
        {
            m_position.p_a = m_body_a->GetCentroid();
            m_position.o_a = m_body_a->GetOrientation();
        }
        if (m_body_b != nullptr)
        {
            m_position.p_b = m_body_b->GetCentroid();
            m_position.o_b = m_body_b->GetOrientation();
        }
        E5_UNUSED_PARAM(dt);
    }

    void ContactConstraint::SolveVelocityConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        for (size_t i = 0; i < m_count; ++i)
        {
            // Solve tangent constraints first because non-penetration is more important than friction.
            SolveJacobianVelocity(m_manifold->contacts[i], m_tangent[i], i);
            SolveJacobianVelocity(m_manifold->contacts[i], m_bitangent[i], i);
            // Solve normal constraints
            SolveJacobianVelocity(m_manifold->contacts[i], m_normal[i], i, true);
        }
    }

    void ContactConstraint::SolvePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        for (auto& contact : m_manifold->contacts)
        {
            Real    separation = DotProduct(contact.global_position_b - contact.global_position_a, contact.normal) - Physics::Collision::SEPARATION_SLOP;
            Real    c          = Math::Clamp(Physics::Dynamics::BAUMGRATE * (separation + Physics::Collision::LINEAR_SLOP), -Physics::Collision::MAX_LINEAR_CORRECTION, 0.0f);
            Vector3 r_a        = contact.global_position_a - m_position.p_a;
            Vector3 r_b        = contact.global_position_b - m_position.p_b;
            Vector3 ra_n       = CrossProduct(contact.r_a, contact.normal);
            Vector3 rb_n       = CrossProduct(contact.r_b, contact.normal);
            Real    k          = m_mass.m_a + ra_n * m_mass.i_a * ra_n + m_mass.m_b + rb_n * m_mass.i_b * rb_n;
            Real    impulse    = k > 0.0f ? -c / k : 0.0f;
            Vector3 p          = impulse * contact.normal;
            Vector3 o_a        = m_mass.i_a * CrossProduct(contact.r_a, p);
            Vector3 o_b        = m_mass.i_b * CrossProduct(contact.r_b, p);
            m_position.p_a -= m_mass.m_a * p;
            m_position.p_b += m_mass.m_b * p;
            m_position.o_a.AddRotation(o_a.Unit(), o_a.Length());
            m_position.o_b.AddRotation(o_b.Unit(), o_b.Length());
        }
    }

    void ContactConstraint::ApplyVelocityConstraints()
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_manifold->contacts[i].tangent_lambda   = m_tangent[i].total_lambda;
            m_manifold->contacts[i].bitangent_lambda = m_bitangent[i].total_lambda;
            m_manifold->contacts[i].normal_lambda    = m_normal[i].total_lambda;
        }
        m_manifold->prev_count = m_count;
        //apply body a
        m_body_a->SetLinearVelocity(m_velocity.v_a);
        m_body_a->SetAngularVelocity(m_velocity.w_a);
        //apply body b
        m_body_b->SetLinearVelocity(m_velocity.v_b);
        m_body_b->SetAngularVelocity(m_velocity.w_b);
    }

    void ContactConstraint::ApplyPositionConstraints()
    {
        //apply body a
        m_body_a->SetCentroid(m_position.p_a);
        m_body_a->SetOrientation(m_position.o_a);
        //apply body b
        m_body_b->SetCentroid(m_position.p_b);
        m_body_b->SetOrientation(m_position.o_b);
    }

    void ContactConstraint::Render(PrimitiveRenderer* primitive_renderer, const Color& color) const
    {
        Quaternion no_rotation;
        Vector3    avg_a, avg_b;
        for (auto& contact_point : m_manifold->contacts)
        {
            Vector3 pos_a = contact_point.global_position_a;
            Vector3 pos_b = contact_point.global_position_b;
            primitive_renderer->DrawPrimitive(Sphere(pos_a, no_rotation, 0.1f), eRenderingMode::Face, color);
            primitive_renderer->DrawPrimitive(Sphere(pos_b, no_rotation, 0.1f), eRenderingMode::Face, color);
            primitive_renderer->DrawSegment(pos_a, pos_a + contact_point.normal, color);
            primitive_renderer->DrawSegment(pos_b, pos_b + contact_point.normal, color);
            avg_a += pos_a;
            avg_b += pos_b;
        }
        Real size = static_cast<Real>(m_manifold->contacts.size());
        avg_a /= size;
        avg_b /= size;
        primitive_renderer->DrawPrimitive(Sphere(avg_a, no_rotation, 0.1f), eRenderingMode::Face, color);
        primitive_renderer->DrawPrimitive(Sphere(avg_b, no_rotation, 0.1f), eRenderingMode::Face, color);
        primitive_renderer->DrawSegment(avg_a, avg_a + m_manifold->manifold_normal, color);
        primitive_renderer->DrawSegment(avg_b, avg_b + m_manifold->manifold_normal, color);
    }

    void ContactConstraint::WarmStart()
    {
        size_t count = m_manifold->contacts.size();
        if (m_manifold->prev_count <= count && count > 2)
        {
            Basis normal_basis;
            normal_basis.CalculateBasisApprox(m_manifold->manifold_normal);
            Vector3 normal    = normal_basis.i;
            Vector3 tangent   = normal_basis.j;
            Vector3 bitangent = normal_basis.k;
            for (size_t i = 0; i < m_manifold->prev_count; ++i)
            {
                Vector3 p = m_manifold->contacts[i].normal_lambda * normal
                        + m_manifold->contacts[i].tangent_lambda * tangent
                        + m_manifold->contacts[i].bitangent_lambda * bitangent;
                m_velocity.v_a -= m_mass.m_a * p;
                m_velocity.w_a -= m_mass.i_a * CrossProduct(m_manifold->contacts[i].r_a, p);
                m_velocity.v_b += m_mass.m_b * p;
                m_velocity.w_b += m_mass.i_b * CrossProduct(m_manifold->contacts[i].r_b, p);
            }
        }
    }

    Real ContactConstraint::GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const
    {
        Physics::MaterialCoefficient m_a(a->GetMaterial());
        Physics::MaterialCoefficient m_b(b->GetMaterial());
        return Math::Min(m_a.restitution, m_b.restitution);
    }

    void ContactConstraint::InitializeJacobianVelocity(const ContactPoint& contact, const Vector3& direction, JacobianVelocity& jacobian, Real dt, bool b_normal) const
    {
        jacobian.v_a  = -direction;
        jacobian.w_a  = -CrossProduct(contact.r_a, direction);
        jacobian.v_b  = direction;
        jacobian.w_b  = CrossProduct(contact.r_b, direction);
        jacobian.bias = 0.0f;
        if (b_normal)
        {
            Real    beta              = m_beta;//contact.contact_beta_a * contact.contact_beta_b;
            Real    restitution       = GetRestitution(contact.collider_a, contact.collider_b);
            Vector3 relative_velocity =
                    -m_velocity.v_a
                    - CrossProduct(m_velocity.w_a, contact.r_a)
                    + m_velocity.v_b
                    + CrossProduct(m_velocity.w_b, contact.r_b);
            Real closing_velocity = DotProduct(relative_velocity, direction);
            jacobian.bias         = -(beta / dt) * contact.depth + restitution * closing_velocity;
        }
        else
        {
            jacobian.bias = -m_tangent_speed;
        }
        bool motion_a = m_body_a->GetMotionMode() == eMotionMode::Dynamic;
        bool motion_b = m_body_b->GetMotionMode() == eMotionMode::Dynamic;
        Real k
                = (motion_a ? m_mass.m_a + jacobian.w_a * m_mass.i_a * jacobian.w_a : 0.0f)
                + (motion_b ? m_mass.m_b + jacobian.w_b * m_mass.i_b * jacobian.w_b : 0.0f);
        jacobian.effective_mass = k > 0.0f ? 1.0f / k : 0.0f;;
        jacobian.total_lambda   = 0.0f;
    }

    void ContactConstraint::SolveJacobianVelocity(const ContactPoint& contact, JacobianVelocity& jacobian, size_t i, bool b_normal)
    {
        Vector3 dir = jacobian.v_b;
        // jv = Jacobian * velocity vector
        Real jv =
                DotProduct(jacobian.v_a, m_velocity.v_a)
                + DotProduct(jacobian.w_a, m_velocity.w_a)
                + DotProduct(jacobian.v_b, m_velocity.v_b)
                + DotProduct(jacobian.w_b, m_velocity.w_b);
        // raw lambda
        Real lambda = jacobian.effective_mass * -(jv + jacobian.bias);
        // clamped lambda
        Real old_total_lambda = jacobian.total_lambda;
        if (b_normal)
        {
            //normal - contact resolution : lambda >= 0
            jacobian.total_lambda = Math::Max(0.0f, jacobian.total_lambda + lambda);
        }
        else
        {
            //tangent - friction : -max_friction <= lambda <= max_friction
            auto friction_data    = m_friction_utility->Find(contact.collider_a->GetMaterial(), contact.collider_b->GetMaterial());
            Real max_friction     = friction_data.dynamic_friction * m_normal[i].total_lambda;
            jacobian.total_lambda = Math::Clamp(jacobian.total_lambda + lambda, -max_friction, max_friction);
        }
        lambda = jacobian.total_lambda - old_total_lambda;
        // velocity correction
        m_velocity.v_a += m_mass.m_a * jacobian.v_a * lambda;
        m_velocity.w_a += m_mass.i_a * jacobian.w_a * lambda;
        m_velocity.v_b += m_mass.m_b * jacobian.v_b * lambda;
        m_velocity.w_b += m_mass.i_b * jacobian.w_b * lambda;
    }
}
