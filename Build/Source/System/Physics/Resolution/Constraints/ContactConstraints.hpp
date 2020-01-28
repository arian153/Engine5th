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
        Real restitution;
        Real normal_mass;
        Real normal_impulse;
    };

    class TangentTerm
    {
    public:
        Vector3 tangent;
        Real friction;
        Real tangent_speed;
        Real tangent_mass;
        Real tangent_impulse;
        Real normal_impulse;
    };

    class ContactConstraints final : public Constraints
    {
    public:
        explicit ContactConstraints(ContactManifold* input);
        ~ContactConstraints();

        void SolveConstraints(Real dt) override;
        void ApplyConstraints() override;

        void InitializeContactConstraints(ContactPoint& contact_point);

        void SolveContactManifold();
        void SolveContactPoint(ContactPoint& contact_point);

        void SolveNormalConstraints(const ContactTerm& contact, const MassTerm& mass, VelocityTerm& velocity, NormalTerm& normal) const;
        void SolveTangentConstraints(const ContactTerm& contact, const MassTerm& mass, VelocityTerm& velocity, TangentTerm& tangent) const;
        void WarmStart();

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
        Real m_restitution = 1.0f;
        Real m_friction    = 1.0f;
        Real m_tangent_speed = 0.0f;
    };
}
