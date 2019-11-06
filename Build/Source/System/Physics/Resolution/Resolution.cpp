#include "Resolution.hpp"
#include "Manifold.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::SolveConstraints() const
    {
    }

    void Resolution::SolveContact(Contact* contact, Real dt) const
    {
        auto body_a = contact->collider_a->GetRigidBody();
        auto body_b = contact->collider_b->GetRigidBody();
        if (contact->is_collide == true && contact->is_valid == true)
        {
            Vector3 ca                = body_a->m_position;
            Vector3 cb                = body_b->m_position;
            Vector3 ra                = contact->local_position_a;
            Vector3 rb                = contact->local_position_b;
            Vector3 pa                = ra + ca; //contact.global_position_a;
            Vector3 pb                = rb + cb; //contact.global_position_b;
            Real    constraints_error = (pb - pa).DotProduct(contact->normal);
            constraints_error         = Utility::Max((constraints_error - penetration_slop), 0.0f);
            Vector3 contact_jacobian_la;
            Vector3 contact_jacobian_aa;
            Vector3 contact_jacobian_lb;
            Vector3 contact_jacobian_ab;
            if (body_a->m_motion_mode == MotionMode::Dynamic)
            {
                contact_jacobian_la = -contact->normal;  // -(n)T;
                contact_jacobian_aa = -ra.CrossProduct(contact->normal); // (-ra X n)T;
            }
            if (body_b->m_motion_mode == MotionMode::Dynamic)
            {
                contact_jacobian_lb = contact->normal;  //  (n)T;
                contact_jacobian_ab = rb.CrossProduct(contact->normal); // (rb X n)T;
            }
            Real effective_mass = contact_jacobian_la * body_a->InverseMassMatrix() * contact_jacobian_la
                    + contact_jacobian_aa * body_a->InverseInertia() * contact_jacobian_aa
                    + contact_jacobian_lb * body_b->InverseMassMatrix() * contact_jacobian_lb
                    + contact_jacobian_ab * body_b->InverseInertia() * contact_jacobian_ab;
            Vector3 relative_velocity_a = body_a->m_linear_velocity + body_a->m_angular_velocity.CrossProduct(ra);
            Vector3 relative_velocity_b = body_b->m_linear_velocity + body_b->m_angular_velocity.CrossProduct(rb);
            Vector3 relative_velocity   = relative_velocity_b - relative_velocity_a;
            Real    projection          = relative_velocity.DotProduct(contact->normal);
            if (projection > 0.0f)
            {
            }
            else
            {
                //Real restitution      = (collider_primitive_a->coef.restitution + collider_primitive_b->coef.restitution) * 0.5f;
                //Real restitution_term = restitution * projection;
                Real baumgrate_term = (-baumgarte_scalar * constraints_error) / dt;
                Real bias_term      = (baumgrate_term /*+ restitution_term*/) / dt;
                Real velocity_term  = contact_jacobian_la * (body_a->m_linear_velocity)
                        + contact_jacobian_aa * (body_a->m_angular_velocity)
                        + contact_jacobian_lb * (body_b->m_linear_velocity)
                        + contact_jacobian_ab * (body_b->m_angular_velocity);
                velocity_term /= dt;
                Real force_term = contact_jacobian_la * body_a->InverseMassMatrix() * body_a->m_force_accumulator
                        + contact_jacobian_aa * body_a->InverseInertia() * body_a->m_torque_accumulator
                        + contact_jacobian_lb * body_b->InverseMassMatrix() * body_b->m_force_accumulator
                        + contact_jacobian_ab * body_b->InverseInertia() * body_b->m_torque_accumulator;
                //Real body_a_term = contact_jacobian_la * manifold.cache_a_positional + contact_jacobian_aa * manifold.cache_a_rotational;
                // Real body_b_term = contact_jacobian_lb * manifold.cache_b_positional + contact_jacobian_ab * manifold.cache_b_rotational;
                Real lambda = -(bias_term + velocity_term + force_term/* - body_a_term - body_b_term*/);
                lambda /= effective_mass;
                Real normal_impulse_sum_copy = contact->normal_impulse_sum;
                contact->normal_impulse_sum += lambda;
                contact->normal_impulse_sum = Utility::Clamp(contact->normal_impulse_sum, 0.0f, Math::REAL_POSITIVE_MAX);
                lambda                      = contact->normal_impulse_sum - normal_impulse_sum_copy;
                //manifold.cache_a_positional += lambda * body_a->InverseMassMatrix() * contact_jacobian_la;
                //manifold.cache_a_rotational += lambda * body_a->InverseInertia() * contact_jacobian_aa;
                //manifold.cache_b_positional += lambda * body_b->InverseMassMatrix() * contact_jacobian_lb;
                //manifold.cache_b_rotational += lambda * body_b->InverseInertia() * contact_jacobian_ab;
                if (lambda > Math::EPSILON)
                {
                    Vector3 force_a  = contact_jacobian_la * lambda * dt;
                    Vector3 torque_a = contact_jacobian_aa * lambda * dt;
                    Vector3 force_b  = contact_jacobian_lb * lambda * dt;
                    Vector3 torque_b = contact_jacobian_ab * lambda * dt;
                    body_a->m_linear_velocity += body_a->InverseMassMatrix() * force_a;
                    body_b->m_linear_velocity += body_b->InverseMassMatrix() * force_b;
                    body_a->m_angular_velocity += (body_a->InverseInertia() * torque_a);
                    body_b->m_angular_velocity += (body_b->InverseInertia() * torque_b);
                    body_a->m_force_accumulator += force_a / dt;
                    body_b->m_force_accumulator += force_b / dt;
                    body_a->m_torque_accumulator += torque_a;
                    body_b->m_torque_accumulator += torque_b;
                }
            }
        }
    }

    void Resolution::SolveManifold() const
    {
    }

    void Resolution::WarmStart(Manifold* manifold) const
    {
        auto body_a = manifold->collider_a->GetRigidBody();
        auto body_b = manifold->collider_b->GetRigidBody();
        auto va     = body_a->GetLinearVelocity();
        auto wa     = body_a->GetAngularVelocity();
        auto vb     = body_b->GetLinearVelocity();
        auto wb     = body_b->GetAngularVelocity();
        auto ma     = body_a->InverseMass();
        auto ia     = body_a->InverseInertia();
        auto mb     = body_b->InverseMass();
        auto ib     = body_b->InverseInertia();
        for (auto& contact : manifold->contacts)
        {
            Vector3 p = contact.normal_impulse_sum * contact.normal
                    + contact.tangent_a_impulse_sum * contact.tangent_a
                    + contact.tangent_b_impulse_sum * contact.tangent_b;
            //-= or +=
            va += ma * p;
            wa += ia * CrossProduct(contact.local_position_a, p);
            //+= or -=
            vb -= mb * p;
            wb -= ib * CrossProduct(contact.local_position_b, p);
        }
    }
}
