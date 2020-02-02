#pragma once
#include "../Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace Engine5
{
    class ColliderComponent final : public Component
    {
    public:
        ColliderComponent();
        ~ColliderComponent();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        ColliderPrimitive* AddCollider(eColliderType type) const;
        ColliderPrimitive* GetCollider(size_t index) const;
        void EraseCollider(ColliderPrimitive* collider) const;

        void SetMass(Real density) const;
        void SetScale(const Vector3& scale) const;

        MassData GetMass() const;
        Vector3 GetScale() const;

    private:
        ColliderSet* m_collider_set = nullptr;
        RigidBody* m_rigid_body = nullptr;
        Transform* m_transform = nullptr;

        bool m_b_init = false;
    };
}
