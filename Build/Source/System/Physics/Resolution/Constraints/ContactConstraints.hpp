#pragma once
#include "../../../Math/Math.hpp"
#include "Constraints.hpp"

namespace Engine5
{
    class ContactManifold;
    class RigidBody;

    class ContactConstraints final : public Constraints
    {
    public:
        ContactConstraints(ContactManifold* input);
        ~ContactConstraints();

        void SolveConstraints(Real dt) override;
        void ApplyConstraints() override;

        void SolveContactManifold();
        void SolveContactPoint();
        void WarmStart();

    private:
        RigidBody* m_body_a = nullptr;
        RigidBody* m_body_b = nullptr;

        ContactManifold* m_manifold = nullptr;

        //velocities
        Vector3 m_v_a;
        Vector3 m_w_a;
        Vector3 m_v_b;
        Vector3 m_w_b;

        //friction and restitution factor.
        //if 1.0f no effects.
        Real m_restitution = 1.0f;
        Real m_friction = 1.0f;
    };
}
