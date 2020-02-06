#pragma once
#include "../../../Math/Math.hpp"
#include "Constraints.hpp"
#include "../../Utility/FrictionUtility.hpp"

namespace Engine5
{
    class ColliderPrimitive;
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

    class PositionTerm
    {
    public:
        Vector3    p_a; //global centroid
        Vector3    p_b; //global centroid
        Quaternion o_a; //global orientation
        Quaternion o_b; //global orientation
    };

    class MassTerm
    {
    public:
        Real     m_a = 0.0f;
        Real     m_b = 0.0f;
        Matrix33 i_a;
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
        explicit ContactConstraints(ContactManifold* input, Physics::FrictionUtility& friction_utility, Real tangent_speed = 0.0f);
        ~ContactConstraints();

        void Initialize() override;
        void Solve(Real dt) override;
        void Apply() override;

        void InitializeContactPoint(ContactPoint& contact_point) const;
        void SolveContactPoint(ContactPoint& contact_point);

        void SolveNormalConstraints(const MassTerm& mass, VelocityTerm& velocity, ContactPoint& contact_point) const;
        void SolveTangentConstraints(const MassTerm& mass, Real tangent_speed, VelocityTerm& velocity, ContactPoint& contact_point) const;
        void WarmStart();

        static void SolvePositionConstraints(const ContactManifold& manifold);

        Real GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const;

    private:
        Physics::FrictionUtility& m_friction_utility;
        ContactManifold* m_manifold = nullptr;
        VelocityTerm     m_velocity;
        MassTerm         m_mass;
        Real             m_tangent_speed = 0.0f;
    };
}
