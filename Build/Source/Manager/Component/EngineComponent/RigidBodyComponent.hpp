#pragma once
#include "..//Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace Engine5
{
    class RigidBodyComponent final : public Component
    {
    public:
        ~RigidBodyComponent();
        RigidBodyComponent()                                         = delete;
        RigidBodyComponent(const RigidBodyComponent& rhs)            = delete;
        RigidBodyComponent& operator=(const RigidBodyComponent& rhs) = delete;

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
        void SetMotionMode(eMotionMode motion_mode) const;
        void SetTransform(Transform* transform);

        //getter
        Vector3     GetLinearVelocity() const;
        Vector3     GetAngularVelocity() const;
        Real        GetMass() const;
        Real        GetInverseMass() const;
        Matrix33    GetMassMatrix() const;
        Matrix33    GetInverseMassMatrix() const;
        Matrix33    GetInertia() const;
        Matrix33    GetInverseInertia() const;
        eMotionMode GetMotionMode() const;
        RigidBody* GetRigidBody() const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit() override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class RigidBodyFactory;
        friend class ColliderComponent;
        friend class RigidBody;

    private:
        explicit RigidBodyComponent(Object* owner);
        void     Clone(RigidBodyComponent* origin);

    private:
        RigidBody*   m_rigid_body   = nullptr;
        ColliderSet* m_collider_set = nullptr;
        Transform*   m_transform    = nullptr;

        bool m_b_init = false;
    };
}
