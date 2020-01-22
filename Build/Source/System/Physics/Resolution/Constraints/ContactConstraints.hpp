#pragma once
#include "../../../Math/Math.hpp"
#include "Constraints.hpp"

namespace Engine5
{
    class RigidBody;

    class ContactConstraints final : public Constraints
    {
    public:
        ContactConstraints();
        ~ContactConstraints();

        void SolveConstraints(Real dt) override;
        void ApplyConstraints() override;

        void SolveContactManifold();
        void SolveContactPoint();
        void WarmStart();

    private:
        RigidBody* body_a = nullptr;
        RigidBody* body_b = nullptr;

        //velocities
        Vector3 m_v_a;
        Vector3 m_w_a;
        Vector3 m_v_b;
        Vector3 m_w_b;

        //friction and restitution factor.
        //if 1.0f no effects.
        Real restitution = 1.0f;
        Real friction = 1.0f;
    };
}
