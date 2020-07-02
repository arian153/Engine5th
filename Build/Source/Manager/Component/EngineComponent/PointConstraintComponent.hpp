#pragma once
#include "..//Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace Engine5
{
    class PointConstraint;

    class PointConstraintComponent final : public Component
    {
    public:
        ~PointConstraintComponent();
        PointConstraintComponent()                                               = delete;
        PointConstraintComponent(const PointConstraintComponent& rhs)            = delete;
        PointConstraintComponent& operator=(const PointConstraintComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        void SetConstraintMode(eConstraintMode mode) const;
        void SetFrequency(Real frequency) const;
        void SetDampingRatio(Real damping_ratio) const;
        void EnableRotation(bool b_rotation) const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class RigidBodyFactory;
        friend class ColliderComponent;
        friend class RigidBody;

    private:
        explicit PointConstraintComponent(Object* owner);
        void     Clone(PointConstraintComponent* origin);

    private:
        RigidBody*       m_rigid_body       = nullptr;
        Transform*       m_transform        = nullptr;
        PointConstraint* m_point_constraint = nullptr;

        bool m_b_init = false;
    };
}
