#include "ContactConstraints.hpp"
#include "../../Dynamics/RigidBody.hpp"

namespace Engine5
{
    ContactConstraints::ContactConstraints()
    {
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
        body_a->SetLinearVelocity(m_v_a);
        body_a->SetAngularVelocity(m_w_a);

        //apply body b
        body_b->SetLinearVelocity(m_v_b);
        body_b->SetAngularVelocity(m_w_b);
    }

    void ContactConstraints::SolveContactManifold()
    {
    }

    void ContactConstraints::SolveContactPoint()
    {
    }

    void ContactConstraints::WarmStart()
    {

    }

}