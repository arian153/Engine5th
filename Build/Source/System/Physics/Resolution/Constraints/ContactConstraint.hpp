#pragma once
#include "../../../Math/Math.hpp"
#include "Constraint.hpp"
#include "../../Utility/FrictionUtility.hpp"

namespace Engine5
{
    class ColorFlag;
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

    class ContactConstraint final : public Constraint
    {
    public:
        explicit ContactConstraint(ContactManifold* input, Physics::FrictionUtility* friction_utility, Real tangent_speed = 0.0f);
        ~ContactConstraint();

        void Initialize() override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void Apply() override;
        void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const;

        void InitializeContactPoint(ContactPoint& contact_point) const;
        void SolveContactPoint(ContactPoint& contact_point);

        void SolveNormalConstraints(const MassTerm& mass, VelocityTerm& velocity, ContactPoint& contact_point) const;
        void SolveTangentConstraints(const MassTerm& mass, Real tangent_speed, VelocityTerm& velocity, ContactPoint& contact_point) const;
        void WarmStart();

        void SolvePositionConstraints();

        static void SolvePositionConstraints(const ContactManifold& manifold);

        Real GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const;

    private:
        Physics::FrictionUtility* m_friction_utility = nullptr;
        ContactManifold*          m_manifold         = nullptr;
        PositionTerm              m_position;
        VelocityTerm              m_velocity;
        MassTerm                  m_mass;
        Real                      m_tangent_speed = 0.0f;
    };
}
