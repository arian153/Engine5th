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

    void ContactConstraints::SolveContactPoint()
    {
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
