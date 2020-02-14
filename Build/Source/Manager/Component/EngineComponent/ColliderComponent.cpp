#include "ColliderComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    ColliderFactory::ColliderFactory()
    {
    }

    ColliderFactory::~ColliderFactory()
    {
    }

    Component* ColliderFactory::Create(Object* owner)
    {
        return new ColliderComponent(owner);
    }

    Component* ColliderFactory::Clone(Component* origin, Object* dest)
    {
        auto source = static_cast<ColliderComponent*>(origin);
        auto cloned = static_cast<ColliderComponent*>(this->Create(dest));
        source->Clone(cloned);
        return cloned;
    }

    ColliderComponent::~ColliderComponent()
    {
    }

    void ColliderComponent::Initialize()
    {
        //m_rigid_body = GetRigidBody();
        if (m_rigid_body != nullptr)
        {
            m_collider_set->Initialize(m_rigid_body);
            m_b_init = true;
        }
    }

    void ColliderComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void ColliderComponent::Shutdown()
    {
        m_collider_set->Shutdown();
    }

    ColliderPrimitive* ColliderComponent::AddCollider(eColliderType type) const
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

    void ColliderComponent::SetMass(Real density) const
    {
        m_collider_set->SetMass(density);
    }

    void ColliderComponent::SetScale(const Vector3& scale) const
    {
        m_collider_set->SetScale(scale);
        if (m_transform != nullptr)
        {
            m_transform->scale = scale;
        }
    }

    MassData ColliderComponent::GetMass() const
    {
        return m_collider_set->GetMassData();
    }

    Vector3 ColliderComponent::GetScale() const
    {
        return m_collider_set->GetScale();
    }

    void ColliderComponent::Load()
    {
    }

    void ColliderComponent::Unload()
    {
    }

    void ColliderComponent::Subscribe()
    {
    }

    void ColliderComponent::Unsubscribe()
    {
    }

    ColliderComponent::ColliderComponent(Object* owner)
        : Component(owner)
    {
    }

    void ColliderComponent::Clone(ColliderComponent* cloned)
    {
        if (cloned != nullptr && cloned != this)
        {
            //copy data
        }
    }
}
