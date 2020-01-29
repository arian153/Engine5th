#include "ContactConstraints.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../ContactManifold.hpp"
#include "../ContactPoint.hpp"
#include "../../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    ContactConstraints::ContactConstraints(ContactManifold* input)
        : m_manifold(input)
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
    }

    void ContactConstraints::SolveConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        InitializeConstraints();

        //set iteration.
        WarmStart();

        SolveContactManifold();

        ApplyConstraints();


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
        for(auto& contact : m_manifold->contacts)
        {
            SolveContactPoint(contact);
        }
    }

    void ContactConstraints::SolveContactPoint(ContactPoint& contact_point)
    {
        RigidBody* body_a = contact_point.collider_a->GetRigidBody();
        RigidBody* body_b = contact_point.collider_b->GetRigidBody();
        //contact term
        ContactTerm contact;
        contact.c_a      = body_a->GetCentroid(); //global centroid.
        contact.c_b      = body_b->GetCentroid(); //global centroid.
        contact.r_a      = contact_point.global_position_a - contact.c_a;
        contact.r_b      = contact_point.global_position_b - contact.c_b;
        Vector3 n        = contact_point.normal;
        Vector3 ta       = contact_point.tangent_a;
        Vector3 tb       = contact_point.tangent_b;
        Vector3 ra_ta    = CrossProduct(contact.r_a, ta);
        Vector3 rb_ta    = CrossProduct(contact.r_b, ta);
        Vector3 ra_tb    = CrossProduct(contact.r_a, tb);
        Vector3 rb_tb    = CrossProduct(contact.r_b, tb);
        Vector3 ra_n     = CrossProduct(contact.r_a, n);
        Vector3 rb_n     = CrossProduct(contact.r_b, n);
        bool    motion_a = body_a->GetMotionMode() == MotionMode::Dynamic;
        bool    motion_b = body_b->GetMotionMode() == MotionMode::Dynamic;
        //tangent term a
        TangentTerm tangent_a;
        tangent_a.tangent         = ta;
        tangent_a.tangent_impulse = contact_point.tangent_a_impulse_sum;
        tangent_a.normal_impulse  = contact_point.normal_impulse_sum;
        tangent_a.friction        = m_friction;
        tangent_a.tangent_mass
                = (motion_a ? ta * m_mass.m_a * ta + ra_ta * m_mass.i_a * ra_ta : 0.0f)
                + (motion_b ? ta * m_mass.m_b * ta + rb_ta * m_mass.i_b * rb_ta : 0.0f);
        tangent_a.tangent_mass  = tangent_a.tangent_mass > 0.0f ? tangent_a.tangent_mass : 0.0f;
        tangent_a.tangent_speed = m_tangent_speed;
        //tangent term b
        TangentTerm tangent_b;
        tangent_b.tangent         = tb;
        tangent_b.tangent_impulse = contact_point.tangent_b_impulse_sum;
        tangent_b.normal_impulse  = contact_point.normal_impulse_sum;
        tangent_b.friction        = m_friction;
        tangent_b.tangent_mass
                = (motion_a ? tb * m_mass.m_a * tb + ra_tb * m_mass.i_a * ra_tb : 0.0f)
                + (motion_b ? tb * m_mass.m_b * tb + rb_tb * m_mass.i_b * rb_tb : 0.0f);
        tangent_b.tangent_mass  = tangent_b.tangent_mass > 0.0f ? tangent_b.tangent_mass : 0.0f;
        tangent_b.tangent_speed = m_tangent_speed;
        //normal term
        NormalTerm normal;
        normal.normal         = n;
        normal.normal_impulse = contact_point.normal_impulse_sum;
        normal.restitution    = m_restitution;
        normal.normal_mass
                = (motion_a ? n * m_mass.m_a * n + ra_n * m_mass.i_a * ra_n : 0.0f)
                + (motion_b ? n * m_mass.m_b * n + rb_n * m_mass.i_b * rb_n : 0.0f);
        normal.normal_mass = normal.normal_mass > 0.0f ? normal.normal_mass : 0.0f;
        // Solve tangent constraints first because non-penetration is more important than friction.
        SolveTangentConstraints(contact, m_mass, m_velocity, tangent_a);
        SolveTangentConstraints(contact, m_mass, m_velocity, tangent_b);
        // Solve normal constraints
        SolveNormalConstraints(contact, m_mass, m_velocity, normal);
        contact_point.normal_impulse_sum    = normal.normal_impulse;
        contact_point.tangent_a_impulse_sum = tangent_a.tangent_impulse;
        contact_point.tangent_b_impulse_sum = tangent_b.tangent_impulse;
        ApplyConstraints();
    }

    void ContactConstraints::SolveNormalConstraints(const ContactTerm& contact, const MassTerm& mass, VelocityTerm& velocity, NormalTerm& normal) const
    {
        // Relative velocity at contact
        Vector3 dv = velocity.v_b + CrossProduct(velocity.w_b, contact.r_b) - velocity.v_a - CrossProduct(velocity.w_a, contact.r_a);
        // Compute normal impulse
        Real vn     = DotProduct(dv, normal.normal);
        Real lambda = -normal.normal_mass * (vn - normal.restitution);
        // b2Clamp the accumulated impulse
        Real new_impulse      = Utility::Max(normal.normal_impulse + lambda, 0.0f);
        lambda                = new_impulse - normal.normal_impulse;
        normal.normal_impulse = new_impulse;
        // Apply contact impulse
        Vector3 p = lambda * normal.normal;
        velocity.v_a -= mass.m_a * p;
        velocity.w_a -= mass.i_a * CrossProduct(contact.r_a, p);
        velocity.v_b += mass.m_b * p;
        velocity.w_b += mass.i_b * CrossProduct(contact.r_b, p);
    }

    void ContactConstraints::SolveTangentConstraints(const ContactTerm& contact, const MassTerm& mass, VelocityTerm& velocity, TangentTerm& tangent) const
    {
        Vector3 dv = velocity.v_b + CrossProduct(velocity.w_b, contact.r_b) - velocity.v_a - CrossProduct(velocity.w_a, contact.r_a);
        // Compute tangent force
        Real vt     = DotProduct(dv, tangent.tangent) - tangent.tangent_speed;
        Real lambda = tangent.tangent_mass * (-vt);
        // b2Clamp the accumulated force
        Real max_friction       = tangent.friction * tangent.normal_impulse; //max friction
        Real new_impulse        = Utility::Clamp(tangent.tangent_impulse + lambda, -max_friction, max_friction);
        lambda                  = new_impulse - tangent.tangent_impulse;
        tangent.tangent_impulse = new_impulse;
        // Apply contact impulse
        Vector3 p = lambda * tangent.tangent;
        velocity.v_a -= mass.m_a * p;
        velocity.w_a -= mass.i_a * CrossProduct(contact.r_a, p);
        velocity.v_b += mass.m_b * p;
        velocity.w_b += mass.i_b * CrossProduct(contact.r_b, p);
    }

    void ContactConstraints::WarmStart()
    {
        //body a data.
        Vector3  va = m_body_a->GetLinearVelocity();
        Vector3  wa = m_body_a->GetAngularVelocity();
        Real     ma = m_body_a->InverseMass();
        Matrix33 ia = m_body_a->InverseInertia();
        //body b data.
        Vector3  vb = m_body_b->GetLinearVelocity();
        Vector3  wb = m_body_b->GetAngularVelocity();
        Real     mb = m_body_b->InverseMass();
        Matrix33 ib = m_body_b->InverseInertia();
        Basis    normal_basis;
        normal_basis.CalculateBasisApprox(m_manifold->manifold_normal);
        Vector3 normal    = normal_basis.i;
        Vector3 tangent_a = normal_basis.j;
        Vector3 tangent_b = normal_basis.k;
        for (auto& contact : m_manifold->contacts)
        {
            Vector3 p = contact.normal_impulse_sum * normal
                    + contact.tangent_a_impulse_sum * tangent_a
                    + contact.tangent_b_impulse_sum * tangent_b;
            //-= or +=
            m_velocity.v_a += ma * p;
            m_velocity.w_a += ia * CrossProduct(contact.local_position_a, p);
            //+= or -=
            m_velocity.v_b -= mb * p;
            m_velocity.w_b -= ib * CrossProduct(contact.local_position_b, p);
        }
    }
}
