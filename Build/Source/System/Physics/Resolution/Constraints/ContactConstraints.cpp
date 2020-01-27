#include "ContactConstraints.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../ContactManifold.hpp"
#include "../ContactPoint.hpp"
#include "../../ColliderPrimitive/ColliderPrimitive.hpp"

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

    void ContactConstraints::SolveConstraints(Real dt)
    {
    }

    void ContactConstraints::ApplyConstraints()
    {
        //apply body a
        m_body_a->AddLinearVelocity(m_dv_a);
        m_body_a->AddAngularVelocity(m_dw_a);
        //apply body b
        m_body_b->AddLinearVelocity(m_dv_b);
        m_body_b->AddAngularVelocity(m_dw_b);
    }

    void ContactConstraints::SolveContactManifold()
    {
    }

    void ContactConstraints::SolveContactPoint(ContactPoint& contact_point)
    {
        RigidBody* body_a = contact_point.collider_a->GetRigidBody();
        RigidBody* body_b = contact_point.collider_b->GetRigidBody();
        //mass term
        MassTerm mass;
        mass.m_a = body_a->InverseMassMatrix();
        mass.i_a = body_a->InverseInertia();
        mass.m_b = body_b->InverseMassMatrix();
        mass.i_b = body_b->InverseInertia();
        //velocity term
        VelocityTerm velocity;
        velocity.v_a = body_a->GetLinearVelocity();
        velocity.w_a = body_a->GetAngularVelocity();
        velocity.v_b = body_b->GetLinearVelocity();
        velocity.w_b = body_b->GetAngularVelocity();
        //other data
        ContactTerm contact;
        contact.c_a = Vector3();  //ToDo - need to calculation
        contact.c_b = Vector3();  //ToDo - need to calculation
        contact.r_a = Vector3();  //ToDo - need to calculation
        contact.r_b = Vector3();  //ToDo - need to calculation
        TangentTerm tangent_a;
        tangent_a.friction        = 0.0f; //ToDo - need to calculation
        tangent_a.normal_impulse  = contact_point.normal_impulse_sum;
        tangent_a.tangent         = contact_point.tangent_a;
        tangent_a.tangent_impulse = contact_point.tangent_a_impulse_sum;
        tangent_a.tangent_mass    = 0.0f; //ToDo - need to calculation
        tangent_a.tangent_speed   = 0.0f; //ToDo - need to set
        TangentTerm tangent_b;
        tangent_b.friction        = 0.0f; //ToDo - need to calculation
        tangent_b.normal_impulse  = contact_point.normal_impulse_sum;
        tangent_b.tangent         = contact_point.tangent_b;
        tangent_b.tangent_impulse = contact_point.tangent_b_impulse_sum;
        tangent_b.tangent_mass    = 0.0f; //ToDo - need to calculation
        tangent_b.tangent_speed   = 0.0f; //ToDo - need to set
        NormalTerm normal;
        normal.normal         = contact_point.normal;
        normal.normal_impulse = contact_point.normal_impulse_sum;
        normal.normal_mass    = 0.0f;  //ToDo - need to calculation
        normal.restitution    = 0.0f;  //ToDo - need to calculation
        //
        // Solve tangent constraints first because non-penetration is more important than friction.
        SolveTangentConstraints(contact, mass, velocity, tangent_a);
        SolveTangentConstraints(contact, mass, velocity, tangent_b);
        // Solve normal constraints
        SolveNormalConstraints(contact, mass, velocity, normal);
        contact_point.normal_impulse_sum    = normal.normal_impulse;
        contact_point.tangent_a_impulse_sum = tangent_a.tangent_impulse;
        contact_point.tangent_b_impulse_sum = tangent_b.tangent_impulse;
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
            m_dv_a += ma * p;
            m_dw_a += ia * CrossProduct(contact.local_position_a, p);
            //+= or -=
            m_dv_b -= mb * p;
            m_dw_b -= ib * CrossProduct(contact.local_position_b, p);
        }
    }
}
