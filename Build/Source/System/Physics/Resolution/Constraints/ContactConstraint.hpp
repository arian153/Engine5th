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

    class ContactConstraint final : public Constraint
    {
    private:
        class Jacobian
        {
        public:
            Vector3 v_a;
            Vector3 w_a;
            Vector3 v_b;
            Vector3 w_b;
            Real    bias           = 0.0f;
            Real    effective_mass = 0.0f;
            Real    m_total_lambda = 0.0f;
        };

        class VelocityData
        {
        public:
            Vector3 v_a;
            Vector3 w_a;
            Vector3 v_b;
            Vector3 w_b;
        };

        class PositionData
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

    public:
        explicit ContactConstraint(ContactManifold* input, Physics::FrictionUtility* friction_utility, Real tangent_speed = 0.0f);
        ~ContactConstraint();

        void GenerateVelocityConstraints() override;
        void GeneratePositionConstraints() override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;
        void ApplyPositionConstraints() override;

        void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const;

        void InitializeContactPoint(ContactPoint& contact_point) const;
        void SolveNormalConstraints(const MassTerm& mass, VelocityData& velocity, ContactPoint& contact_point) const;
        void SolveTangentConstraints(const MassTerm& mass, Real tangent_speed, VelocityData& velocity, ContactPoint& contact_point) const;
        void WarmStart();

        Real GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const;

    private:
        Physics::FrictionUtility* m_friction_utility = nullptr;
        ContactManifold*          m_manifold         = nullptr;
        PositionData              m_position;
        VelocityData              m_velocity;
        MassTerm                  m_mass;
        Real                      m_tangent_speed = 0.0f;

        Jacobian m_normal;
        Jacobian m_tangent;
        Jacobian m_bitangent;

        RigidBody* m_body_a = nullptr;
        RigidBody* m_body_b = nullptr;
    };
}
