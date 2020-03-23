#include "ColliderComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../Object/Object.hpp"
#include "RigidBodyComponent.hpp"

namespace Engine5
{
    ColliderComponent::~ColliderComponent()
    {
    }

    void ColliderComponent::Initialize()
    {
        if (m_collider_set == nullptr)
        {
            World* world   = m_space != nullptr ? m_space->GetWorld() : nullptr;
            m_collider_set = new ColliderSet(world);
            Subscribe();
        }
        if (m_rigid_body != nullptr)
        {
            m_collider_set->Initialize(m_rigid_body);
            m_b_init = true;
        }
        else
        {
            if (m_owner->HasComponent<RigidBodyComponent>())
            {
                auto body            = m_owner->GetComponent<RigidBodyComponent>();
                m_rigid_body         = body->m_rigid_body;
                body->m_collider_set = m_collider_set;
                body->Initialize();
            }
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

    void ColliderComponent::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    bool ColliderComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void ColliderComponent::Save(Json::Value& data) const
    {
    }

    void ColliderComponent::Subscribe()
    {
        if (m_space != nullptr && m_collider_set != nullptr)
        {
            m_space->GetWorld()->AddColliderSet(m_collider_set);
        }
    }

    void ColliderComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetWorld()->RemoveColliderSet(m_collider_set);
        }
    }

    ColliderComponent::ColliderComponent(Object* owner)
        : Component(owner)
    {
    }

    void ColliderComponent::Clone(ColliderComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
            Initialize();
            m_collider_set->m_mass_data= origin->m_collider_set->m_mass_data;
            origin->m_collider_set->m_scale     = m_collider_set->m_scale;

            //need to copy collider.

            if(m_b_init == true)
            {
                
            }
        }
    }
}
