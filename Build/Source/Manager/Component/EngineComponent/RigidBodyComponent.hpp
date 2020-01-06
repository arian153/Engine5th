#pragma once
#include "..//Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace Engine5
{
    class RigidBodyComponent final : public Component
    {
    public:
        RigidBodyComponent();
        ~RigidBodyComponent();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        //setter
        void ApplyForce(const Vector3& force, const Vector3& at) const;
        void ApplyForceCentroid(const Vector3& force) const;
        void SetLinearVelocity(const Vector3& linear) const;
        void SetAngularVelocity(const Vector3& angular) const;
        void SetMassInfinite() const;
        void SetMass(Real mass = 1.0f) const;
        void SetPositionalConstraints(const Vector3& linear) const;
        void SetRotationalConstraints(const Vector3& angular) const;
        void SetMotionMode(MotionMode motion_mode) const;

        //getter
        Vector3    GetLinearVelocity() const;
        Vector3    GetAngularVelocity() const;
        Real       GetMass() const;
        Real       GetInverseMass() const;
        Matrix33   GetMassMatrix() const;
        Matrix33   GetInverseMassMatrix() const;
        Matrix33   GetInertia() const;
        Matrix33   GetInverseInertia() const;
        MotionMode GetMotionMode() const;


    private:
        RigidBody* m_rigid_body = nullptr;
    };
}
