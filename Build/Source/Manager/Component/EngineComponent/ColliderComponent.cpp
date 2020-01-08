#include "ColliderComponent.hpp"

namespace Engine5
{
    ColliderComponent::ColliderComponent()
    {
    }

    ColliderComponent::~ColliderComponent()
    {
    }

    void ColliderComponent::Initialize()
    {
        m_collider_set->Initialize();
    }

    void ColliderComponent::Update(Real dt)
    {
    }

    void ColliderComponent::Shutdown()
    {
        m_collider_set->Shutdown();
    }

    ColliderPrimitive* ColliderComponent::AddCollider(ColliderType type) const
    {
        return m_collider_set->AddCollider(type);
    }

    ColliderPrimitive* ColliderComponent::GetCollider(size_t index) const
    {
        return m_collider_set->GetCollider(index);
    }

    void ColliderComponent::EraseCollider(ColliderPrimitive* collider) const
    {
        m_collider_set->EraseCollider(collider);
    }
}
