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

    void ContactConstraint::GenerateVelocityConstraints()
    {
        RigidBody* body_a = m_manifold->m_set_a->GetRigidBody();
        RigidBody* body_b = m_manifold->m_set_b->GetRigidBody();
        //mass term
        m_mass.m_a = body_a->Mass();
        m_mass.i_a = body_a->Inertia();
        m_mass.m_b = body_b->Mass();
        m_mass.i_b = body_b->Inertia();
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

    void ContactConstraint::GeneratePositionConstraints()
    {
    }

    void ContactConstraint::SolveVelocityConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        for (auto& contact : m_manifold->contacts)
        {
            SolveContactPoint(contact);
        }
    }

    void ContactConstraint::SolvePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void ContactConstraint::ApplyVelocityConstraints()
    {
        RigidBody* body_a = m_manifold->m_set_a->GetRigidBody();
        RigidBody* body_b = m_manifold->m_set_b->GetRigidBody();
        //apply body a
        body_a->SetLinearVelocity(m_velocity.v_a);
        body_a->SetAngularVelocity(m_velocity.w_a);
        //apply body b
        body_b->SetLinearVelocity(m_velocity.v_b);
        body_b->SetAngularVelocity(m_velocity.w_b);
    }

    void ContactConstraint::ApplyPositionConstraints()
    {
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

    void ContactConstraint::InitializeContactPoint(ContactPoint& contact_point) const
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
        bool    motion_a  = body_a->GetMotionMode() == eMotionMode::Dynamic;
        bool    motion_b  = body_b->GetMotionMode() == eMotionMode::Dynamic;
        Real    tangent_a_mass
                = (motion_a ? m_mass.m_a + ra_ta * m_mass.i_a * ra_ta : 0.0f)
                + (motion_b ? m_mass.m_b + rb_ta * m_mass.i_b * rb_ta : 0.0f);
        contact_point.tangent_a_mass = tangent_a_mass > 0.0f ? 1.0f / tangent_a_mass : 0.0f;
        Real tangent_b_mass
                = (motion_a ? m_mass.m_a + ra_tb * m_mass.i_a * ra_tb : 0.0f)
                + (motion_b ? m_mass.m_b + rb_tb * m_mass.i_b * rb_tb : 0.0f);
        contact_point.tangent_b_mass = tangent_b_mass > 0.0f ? 1.0f / tangent_b_mass : 0.0f;
        Real normal_mass
                = (motion_a ? m_mass.m_a + ra_n * m_mass.i_a * ra_n : 0.0f)
                + (motion_b ? m_mass.m_b + rb_n * m_mass.i_b * rb_n : 0.0f);
        contact_point.normal_mass   = normal_mass > 0.0f ? 1.0f / normal_mass : 0.0f;
        contact_point.velocity_bias = 0.0f;
        Real rel                    = DotProduct(
                                                 n, m_velocity.v_b + CrossProduct(m_velocity.w_b, contact_point.r_b)
                                                 - m_velocity.v_a - CrossProduct(m_velocity.w_a, contact_point.r_a));
        if (rel < -Physics::Dynamics::ELASTIC_THRESHOLD)
        {
            Real restitution            = GetRestitution(contact_point.collider_a, contact_point.collider_b);
            contact_point.velocity_bias = -restitution * rel;
        }
    }

    void ContactConstraint::SolveContactPoint(ContactPoint& contact_point)
    {
        RigidBody* body_a = m_manifold->m_set_a->GetRigidBody();
        RigidBody* body_b = m_manifold->m_set_b->GetRigidBody();
        m_mass.m_a        = body_a->Mass();
        m_mass.i_a        = body_a->Inertia();
        m_mass.m_b        = body_b->Mass();
        m_mass.i_b        = body_b->Inertia();
        // Solve tangent constraints first because non-penetration is more important than friction.
        SolveTangentConstraints(m_mass, m_tangent_speed, m_velocity, contact_point);
        // Solve normal constraints
        SolveNormalConstraints(m_mass, m_velocity, contact_point);
    }

    void ContactConstraint::SolveNormalConstraints(const MassTerm& mass, VelocityTerm& velocity, ContactPoint& contact_point) const
    {
        // Relative velocity at contact
        Vector3 dv = velocity.v_b + CrossProduct(velocity.w_b, contact_point.r_b) - velocity.v_a - CrossProduct(velocity.w_a, contact_point.r_a);
        // Compute normal impulse
        Real vn     = DotProduct(dv, contact_point.normal);
        Real lambda = -contact_point.normal_mass * (vn - contact_point.velocity_bias);
        // b2Clamp the accumulated impulse
        Real new_impulse                 = Math::Max(contact_point.normal_impulse_sum + lambda, 0.0f);
        lambda                           = new_impulse - contact_point.normal_impulse_sum;
        contact_point.normal_impulse_sum = new_impulse;
        // Apply contact impulse
        Vector3 p = lambda * contact_point.normal;
        velocity.v_a -= mass.m_a * p;
        velocity.w_a -= mass.i_a * CrossProduct(contact_point.r_a, p);
        velocity.v_b += mass.m_b * p;
        velocity.w_b += mass.i_b * CrossProduct(contact_point.r_b, p);
    }

    void ContactConstraint::SolveTangentConstraints(const MassTerm& mass, Real tangent_speed, VelocityTerm& velocity, ContactPoint& contact_point) const
    {
        Vector3 dv           = velocity.v_b + CrossProduct(velocity.w_b, contact_point.r_b) - velocity.v_a - CrossProduct(velocity.w_a, contact_point.r_a);
        auto    friction     = m_friction_utility->Find(contact_point.collider_a->GetMaterial(), contact_point.collider_b->GetMaterial());
        Real    max_friction = friction.dynamic_friction * contact_point.normal_impulse_sum; //max friction
        // Compute tangent force
        Real vt_a     = DotProduct(dv, contact_point.tangent_a) - tangent_speed;
        Real lambda_a = contact_point.tangent_a_mass * (-vt_a);
        // b2Clamp the accumulated force
        Real new_impulse_a                  = Math::Clamp(contact_point.tangent_a_impulse_sum + lambda_a, -max_friction, max_friction);
        lambda_a                            = new_impulse_a - contact_point.tangent_a_impulse_sum;
        contact_point.tangent_a_impulse_sum = new_impulse_a;
        // Apply contact impulse
        Vector3 p_a = lambda_a * contact_point.tangent_a;
        // Compute tangent force
        Real vt_b     = DotProduct(dv, contact_point.tangent_b) - tangent_speed;
        Real lambda_b = contact_point.tangent_b_mass * (-vt_b);
        // b2Clamp the accumulated force
        Real new_impulse_b                  = Math::Clamp(contact_point.tangent_b_impulse_sum + lambda_b, -max_friction, max_friction);
        lambda_b                            = new_impulse_b - contact_point.tangent_b_impulse_sum;
        contact_point.tangent_b_impulse_sum = new_impulse_b;
        // Apply contact impulse
        Vector3 p_b = lambda_b * contact_point.tangent_a;
        velocity.v_a -= mass.m_a * (p_a + p_b);
        velocity.w_a -= mass.i_a * (CrossProduct(contact_point.r_a, p_a) + CrossProduct(contact_point.r_a, p_b));
        velocity.v_b += mass.m_b * (p_a + p_b);
        velocity.w_b += mass.i_b * (CrossProduct(contact_point.r_b, p_a) + CrossProduct(contact_point.r_b, p_b));
    }

    void ContactConstraint::WarmStart()
    {
        Basis normal_basis;
        normal_basis.CalculateBasisApprox(m_manifold->manifold_normal);
        Vector3    normal    = normal_basis.i;
        Vector3    tangent_a = normal_basis.j;
        Vector3    tangent_b = normal_basis.k;
        RigidBody* body_a    = m_manifold->m_set_a->GetRigidBody();
        RigidBody* body_b    = m_manifold->m_set_b->GetRigidBody();
        m_mass.m_a           = body_a->Mass();
        m_mass.i_a           = body_a->Inertia();
        m_mass.m_b           = body_b->Mass();
        m_mass.i_b           = body_b->Inertia();
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

    void ContactConstraint::SolvePositionConstraints()
    {
        RigidBody* body_a = m_manifold->m_set_a->GetRigidBody();
        RigidBody* body_b = m_manifold->m_set_b->GetRigidBody();
        m_position.p_a    = body_a->GetCentroid();
        m_position.p_b    = body_b->GetCentroid();
        m_position.o_a    = body_a->GetOrientation();
        m_position.o_b    = body_b->GetOrientation();
        m_mass.m_a        = body_a->Mass();
        m_mass.i_a        = body_a->Inertia();
        m_mass.m_b        = body_b->Mass();
        m_mass.i_b        = body_b->Inertia();
        for (auto& contact : m_manifold->contacts)
        {
            Real    separation = DotProduct((contact.global_position_b - contact.global_position_a), contact.normal) - Physics::Collision::SEPARATION_SLOP;
            Real    c          = Math::Clamp(Physics::Dynamics::BAUMGRATE * (separation + Physics::Collision::LINEAR_SLOP), -Physics::Collision::MAX_LINEAR_CORRECTION, 0.0f);
            Vector3 c_a        = body_a->GetCentroid(); //global centroid.
            Vector3 c_b        = body_b->GetCentroid(); //global centroid.
            Vector3 r_a        = contact.global_position_a - c_a;
            Vector3 r_b        = contact.global_position_b - c_b;
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
        body_a->SetCentroid(m_position.p_a);
        body_b->SetCentroid(m_position.p_b);
        body_a->SetOrientation(m_position.o_a);
        body_b->SetOrientation(m_position.o_b);
    }

    void ContactConstraint::SolvePositionConstraints(const ContactManifold& manifold)
    {
        RigidBody* body_a = manifold.m_set_a->GetRigidBody();
        RigidBody* body_b = manifold.m_set_b->GetRigidBody();
        MassTerm   mass;
        mass.m_a = body_a->Mass();
        mass.i_a = body_a->Inertia();
        mass.m_b = body_b->Mass();
        mass.i_b = body_b->Inertia();
        PositionTerm position;
        position.p_a = body_a->GetCentroid();
        position.p_b = body_b->GetCentroid();
        position.o_a = body_a->GetOrientation();
        position.o_b = body_b->GetOrientation();
        // Solve normal constraints
        for (auto& contact : manifold.contacts)
        {
            Real    separation = DotProduct((contact.global_position_b - contact.global_position_a), contact.normal) - Physics::Collision::SEPARATION_SLOP;
            Real    c          = Math::Clamp(Physics::Dynamics::BAUMGRATE * (separation + Physics::Collision::LINEAR_SLOP), -Physics::Collision::MAX_LINEAR_CORRECTION, 0.0f);
            Vector3 c_a        = body_a->GetCentroid(); //global centroid.
            Vector3 c_b        = body_b->GetCentroid(); //global centroid.
            Vector3 r_a        = contact.global_position_a - c_a;
            Vector3 r_b        = contact.global_position_b - c_b;
            Vector3 ra_n       = CrossProduct(contact.r_a, contact.normal);
            Vector3 rb_n       = CrossProduct(contact.r_b, contact.normal);
            Real    k          = mass.m_a + ra_n * mass.i_a * ra_n + mass.m_b + rb_n * mass.i_b * rb_n;
            Real    impulse    = k > 0.0f ? -c / k : 0.0f;
            Vector3 p          = impulse * contact.normal;
            Vector3 o_a        = mass.i_a * CrossProduct(contact.r_a, p);
            Vector3 o_b        = mass.i_b * CrossProduct(contact.r_b, p);
            position.p_a -= mass.m_a * p;
            position.p_b += mass.m_b * p;
            position.o_a.AddRotation(o_a.Unit(), o_a.Length());
            position.o_b.AddRotation(o_b.Unit(), o_b.Length());
        }
        body_a->SetCentroid(position.p_a);
        body_b->SetCentroid(position.p_b);
        body_a->SetOrientation(position.o_a);
        body_b->SetOrientation(position.o_b);
    }

    Real ContactConstraint::GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const
    {
        Physics::MaterialCoefficient m_a(a->GetMaterial());
        Physics::MaterialCoefficient m_b(b->GetMaterial());
        return Math::Min(m_a.restitution, m_b.restitution);
    }
}
