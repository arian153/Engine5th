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
        m_body_a = m_manifold->m_body_a;
        m_body_b = m_manifold->m_body_b;
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
        m_body_a->SetLinearVelocity(m_v_a);
        m_body_a->SetAngularVelocity(m_w_a);

        //apply body b
        m_body_b->SetLinearVelocity(m_v_b);
        m_body_b->SetAngularVelocity(m_w_b);
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
        auto va = m_body_a->GetLinearVelocity();
        auto wa = m_body_a->GetAngularVelocity();
        auto ma = m_body_a->InverseMass();
        auto ia = m_body_a->InverseInertia();

        //body b data.
        auto vb = m_body_b->GetLinearVelocity();
        auto wb = m_body_b->GetAngularVelocity();
        auto mb = m_body_b->InverseMass();
        auto ib = m_body_b->InverseInertia();

        Basis normal_basis;
        normal_basis.CalculateBasisApprox(m_manifold->manifold_normal);
        Vector3 normal = normal_basis.i;
        Vector3 tangent_a = normal_basis.j;
        Vector3 tangent_b = normal_basis.k;

        for (auto& contact : m_manifold->contacts)
        {
            Vector3 p = contact.normal_impulse_sum * normal
                + contact.tangent_a_impulse_sum * tangent_a
                + contact.tangent_b_impulse_sum * tangent_b;
            //-= or +=
            va += ma * p;
            wa += ia * CrossProduct(contact.local_position_a, p);
            //+= or -=
            vb -= mb * p;
            wb -= ib * CrossProduct(contact.local_position_b, p);
        }

    }

}