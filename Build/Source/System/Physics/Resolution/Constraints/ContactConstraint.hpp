#pragma once
#include "../../../Math/Math.hpp"
#include "Constraint.hpp"
#include "../../Utility/FrictionUtility.hpp"
#include "../../Utility/PhysicsDef.hpp"

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
        class JacobianVelocity
        {
        public:
            Vector3 v_a;
            Vector3 w_a;
            Vector3 v_b;
            Vector3 w_b;
            Real    bias           = 0.0f;
            Real    effective_mass = 0.0f;
            Real    total_lambda   = 0.0f;
        };

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

    public:
        explicit ContactConstraint(ContactManifold* input, Physics::FrictionUtility* friction_utility, Real tangent_speed = 0.0f);
        ~ContactConstraint();

        void GenerateVelocityConstraints(Real dt) override;
        void GeneratePositionConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;
        void ApplyPositionConstraints() override;

        void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const;
        void WarmStart();
        Real GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const;
        void InitializeJacobianVelocity(const ContactPoint& contact, const Vector3& direction, JacobianVelocity& jacobian, Real dt, bool b_normal = false) const;
        void SolveJacobianVelocity(const ContactPoint& contact, JacobianVelocity& jacobian, size_t i, bool b_normal = false);

    private:
        Physics::FrictionUtility* m_friction_utility = nullptr;
        ContactManifold*          m_manifold         = nullptr;
        PositionTerm              m_position;
        VelocityTerm              m_velocity;
        MassTerm                  m_mass;
        Real                      m_tangent_speed = 0.0f;
        Real                      m_beta          = 0.25f;

        JacobianVelocity m_normal[ Physics::Collision::MAX_MANIFOLD_POINT_COUNT ];
        JacobianVelocity m_tangent[ Physics::Collision::MAX_MANIFOLD_POINT_COUNT ];
        JacobianVelocity m_bitangent[ Physics::Collision::MAX_MANIFOLD_POINT_COUNT ];
        size_t           m_count = 0;

        RigidBody* m_body_a = nullptr;
        RigidBody* m_body_b = nullptr;
    };
}
