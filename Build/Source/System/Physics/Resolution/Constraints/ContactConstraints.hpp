#pragma once
#include "../../../Math/Math.hpp"
#include "Constraints.hpp"

namespace Engine5
{
    class ContactPoint;
    class ContactManifold;
    class RigidBody;

    class VelocityTerm
    {
    public:
        Vector3 v_a;
        Vector3 w_a;
        Vector3 v_b;
        Vector3 w_b;
    };

    class MassTerm
    {
    public:
        Matrix33 m_a;
        Matrix33 i_a;
        Matrix33 m_b;
        Matrix33 i_b;
    };

    class ContactTerm
    {
    public:
        Vector3 r_a;
        Vector3 r_b;
        Vector3 c_a;
        Vector3 c_b;
    };

    class NormalTerm
    {
    public:
        Vector3 normal;
        Real    restitution    = 0.0f;
        Real    normal_mass    = 0.0f;
        Real    normal_impulse = 0.0f;
    };

    class TangentTerm
    {
    public:
        Vector3 tangent;
        Real    friction        = 0.0f;
        Real    tangent_speed   = 0.0f;
        Real    tangent_mass    = 0.0f;
        Real    tangent_impulse = 0.0f;
    };

    class ContactConstraints final : public Constraints
    {
    public:
        explicit ContactConstraints(ContactManifold* input, Real friction, Real restitution, Real tangent_speed = 0.0f);
        ~ContactConstraints();

        void InitializeConstraints() override;
        void SolveConstraints(Real dt) override;
        void ApplyConstraints() override;

        void SolveContactManifold();
        void InitializeContactPoint(ContactPoint& contact_point) const;
        void SolveContactPoint(ContactPoint& contact_point);

        void SolveNormalConstraints(const MassTerm& mass, VelocityTerm& velocity, ContactPoint& contact_point) const;
        void SolveTangentConstraints(const MassTerm& mass, Real friction, Real tangent_speed, VelocityTerm& velocity, ContactPoint& contact_point) const;
        void WarmStart();

        void SolvePositionConstraints();

    private:
        RigidBody* m_body_a = nullptr;
        RigidBody* m_body_b = nullptr;

        ContactManifold* m_manifold = nullptr;

        //velocities
        VelocityTerm m_velocity;

        //constants
        MassTerm m_mass;

        //friction and restitution factor.
        //if 1.0f no effects.

        Real m_restitution   = 1.0f;
        Real m_friction      = 1.0f;
        Real m_tangent_speed = 0.0f;
    };
}
