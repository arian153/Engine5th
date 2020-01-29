#include "ContactConstraints.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../ContactManifold.hpp"
#include "../ContactPoint.hpp"
#include "../../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    ContactConstraints::ContactConstraints(ContactManifold* input, Real friction, Real restitution, Real tangent_speed)
        : m_manifold(input), m_restitution(restitution), m_friction(friction), m_tangent_speed(tangent_speed)
    {
        m_body_a = m_manifold->m_set_a->GetRigidBody();
        m_body_b = m_manifold->m_set_b->GetRigidBody();
    }

    ContactConstraints::~ContactConstraints()
    {
    }

    void ContactConstraints::InitializeConstraints()
    {
        RigidBody* body_a = m_manifold->m_set_a->GetRigidBody();
        RigidBody* body_b = m_manifold->m_set_b->GetRigidBody();
        //mass term
        m_mass.m_a = body_a->InverseMassMatrix();
        m_mass.i_a = body_a->InverseInertia();
        m_mass.m_b = body_b->InverseMassMatrix();
        m_mass.i_b = body_b->InverseInertia();
        //velocity term
        m_velocity.v_a = body_a->GetLinearVelocity();
        m_velocity.w_a = body_a->GetAngularVelocity();
        m_velocity.v_b = body_b->GetLinearVelocity();
        m_velocity.w_b = body_b->GetAngularVelocity();
        for (auto& contact : m_manifold->contacts)
        {
            InitializeContactPoint(contact);
        }
    }

    void ContactConstraints::SolveConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        WarmStart();
        //iterate.
        SolveContactManifold();
    }

    void ContactConstraints::ApplyConstraints()
    {
        //apply body a
        m_body_a->SetLinearVelocity(m_velocity.v_a);
        m_body_a->SetAngularVelocity(m_velocity.w_a);
        //apply body b
        m_body_b->SetLinearVelocity(m_velocity.v_b);
        m_body_b->SetAngularVelocity(m_velocity.w_b);
    }

    void ContactConstraints::SolveContactManifold()
    {
        for (auto& contact : m_manifold->contacts)
        {
            SolveContactPoint(contact);
        }
    }

    void ContactConstraints::InitializeContactPoint(ContactPoint& contact_point) const
    {
        RigidBody* body_a = contact_point.collider_a->GetRigidBody();
        RigidBody* body_b = contact_point.collider_b->GetRigidBody();
        contact_point.c_a = body_a->GetCentroid(); //global centroid.
        contact_point.c_b = body_b->GetCentroid(); //global centroid.
        contact_point.r_a = contact_point.global_position_a - contact_point.c_a;
        contact_point.r_b = contact_point.global_position_b - contact_point.c_b;
        Vector3 n         = contact_point.normal;
        Vector3 ta        = contact_point.tangent_a;
        Vector3 tb        = contact_point.tangent_b;
        Vector3 ra_ta     = CrossProduct(contact_point.r_a, ta);
        Vector3 rb_ta     = CrossProduct(contact_point.r_b, ta);
        Vector3 ra_tb     = CrossProduct(contact_point.r_a, tb);
        Vector3 rb_tb     = CrossProduct(contact_point.r_b, tb);
        Vector3 ra_n      = CrossProduct(contact_point.r_a, n);
        Vector3 rb_n      = CrossProduct(contact_point.r_b, n);
        bool    motion_a  = body_a->GetMotionMode() == MotionMode::Dynamic;
        bool    motion_b  = body_b->GetMotionMode() == MotionMode::Dynamic;
        contact_point.tangent_a_mass
                = (motion_a ? ta * m_mass.m_a * ta + ra_ta * m_mass.i_a * ra_ta : 0.0f)
                + (motion_b ? ta * m_mass.m_b * ta + rb_ta * m_mass.i_b * rb_ta : 0.0f);
        contact_point.tangent_a_mass = contact_point.tangent_a_mass > 0.0f ? contact_point.tangent_a_mass : 0.0f;
        contact_point.tangent_b_mass
                = (motion_a ? tb * m_mass.m_a * tb + ra_tb * m_mass.i_a * ra_tb : 0.0f)
                + (motion_b ? tb * m_mass.m_b * tb + rb_tb * m_mass.i_b * rb_tb : 0.0f);
        contact_point.tangent_b_mass = contact_point.tangent_b_mass > 0.0f ? contact_point.tangent_b_mass : 0.0f;
        contact_point.normal_mass
                = (motion_a ? n * m_mass.m_a * n + ra_n * m_mass.i_a * ra_n : 0.0f)
                + (motion_b ? n * m_mass.m_b * n + rb_n * m_mass.i_b * rb_n : 0.0f);
        contact_point.normal_mass = contact_point.normal_mass > 0.0f ? contact_point.normal_mass : 0.0f;
    }

    void ContactConstraints::SolveContactPoint(ContactPoint& contact_point)
    {
        // Solve tangent constraints first because non-penetration is more important than friction.
        SolveTangentConstraints(m_mass, m_friction, m_tangent_speed, m_velocity, contact_point);
        // Solve normal constraints
        SolveNormalConstraints(m_mass, m_restitution, m_velocity, contact_point);
    }

    void ContactConstraints::SolveNormalConstraints(const MassTerm& mass, Real velocity_bias, VelocityTerm& velocity, ContactPoint& contact_point) const
    {
        // Relative velocity at contact
        Vector3 dv = velocity.v_b + CrossProduct(velocity.w_b, contact_point.r_b) - velocity.v_a - CrossProduct(velocity.w_a, contact_point.r_a);
        // Compute normal impulse
        Real vn     = DotProduct(dv, contact_point.normal);
        Real lambda = -contact_point.normal_mass * (vn - velocity_bias);
        // b2Clamp the accumulated impulse
        Real new_impulse                 = Utility::Max(contact_point.normal_impulse_sum + lambda, 0.0f);
        lambda                           = new_impulse - contact_point.normal_impulse_sum;
        contact_point.normal_impulse_sum = new_impulse;
        // Apply contact impulse
        Vector3 p = lambda * contact_point.normal;
        velocity.v_a -= mass.m_a * p;
        velocity.w_a -= mass.i_a * CrossProduct(contact_point.r_a, p);
        velocity.v_b += mass.m_b * p;
        velocity.w_b += mass.i_b * CrossProduct(contact_point.r_b, p);
    }

    void ContactConstraints::SolveTangentConstraints(const MassTerm& mass, Real friction, Real tangent_speed, VelocityTerm& velocity, ContactPoint& contact_point) const
    {
        Vector3 dv           = velocity.v_b + CrossProduct(velocity.w_b, contact_point.r_b) - velocity.v_a - CrossProduct(velocity.w_a, contact_point.r_a);
        Real    max_friction = friction * contact_point.normal_impulse_sum; //max friction
        // Compute tangent force
        Real vt_a     = DotProduct(dv, contact_point.tangent_a) - tangent_speed;
        Real lambda_a = contact_point.tangent_a_mass * (-vt_a);
        // b2Clamp the accumulated force
        Real new_impulse_a                  = Utility::Clamp(contact_point.tangent_a_impulse_sum + lambda_a, -max_friction, max_friction);
        lambda_a                            = new_impulse_a - contact_point.tangent_a_impulse_sum;
        contact_point.tangent_a_impulse_sum = new_impulse_a;
        // Apply contact impulse
        Vector3 p_a = lambda_a * contact_point.tangent_a;
        // Compute tangent force
        Real vt_b     = DotProduct(dv, contact_point.tangent_b) - tangent_speed;
        Real lambda_b = contact_point.tangent_b_mass * (-vt_b);
        // b2Clamp the accumulated force
        Real new_impulse_b                  = Utility::Clamp(contact_point.tangent_b_impulse_sum + lambda_b, -max_friction, max_friction);
        lambda_b                            = new_impulse_b - contact_point.tangent_b_impulse_sum;
        contact_point.tangent_b_impulse_sum = new_impulse_b;
        // Apply contact impulse
        Vector3 p_b = lambda_b * contact_point.tangent_a;
        velocity.v_a -= mass.m_a * (p_a + p_b);
        velocity.w_a -= mass.i_a * (CrossProduct(contact_point.r_a, p_a) + CrossProduct(contact_point.r_a, p_b));
        velocity.v_b += mass.m_b * (p_a + p_b);
        velocity.w_b += mass.i_b * (CrossProduct(contact_point.r_b, p_a) + CrossProduct(contact_point.r_b, p_b));
    }

    void ContactConstraints::WarmStart()
    {
        Basis normal_basis;
        normal_basis.CalculateBasisApprox(m_manifold->manifold_normal);
        Vector3 normal    = normal_basis.i;
        Vector3 tangent_a = normal_basis.j;
        Vector3 tangent_b = normal_basis.k;
        for (auto& contact : m_manifold->contacts)
        {
            Vector3 p = contact.normal_impulse_sum * normal
                    + contact.tangent_a_impulse_sum * tangent_a
                    + contact.tangent_b_impulse_sum * tangent_b;
            m_velocity.v_a -= m_mass.m_a * p;
            m_velocity.w_a -= m_mass.i_a * CrossProduct(contact.r_a, p);
            m_velocity.v_b += m_mass.m_b * p;
            m_velocity.w_b += m_mass.i_b * CrossProduct(contact.r_b, p);
        }
    }
}
