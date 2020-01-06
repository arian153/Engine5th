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
        m_collider_set->Update(dt);
    }

    void ColliderComponent::Shutdown()
    {
        m_collider_set->Shutdown();
    }
}
