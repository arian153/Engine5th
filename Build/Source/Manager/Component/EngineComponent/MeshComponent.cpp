#include "MeshComponent.hpp"
#include "../../../System/Graphics/Element/Mesh.hpp"
#include "../../Object/Object.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"

namespace Engine5
{
    MeshComponent::~MeshComponent()
    {
    }

    void MeshComponent::Initialize()
    {
        if (m_mesh == nullptr)
        {
            m_mesh = new Mesh();
            m_mesh->Initialize();
            m_mesh->m_component = this;
            Subscribe();
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_mesh->SetTransform(m_transform);
            }
        }
    }

    void MeshComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void MeshComponent::Shutdown()
    {
        Unsubscribe();
        if (m_mesh != nullptr)
        {
            m_mesh->Shutdown();
            delete m_mesh;
            m_mesh = nullptr;
        }
    }

    bool MeshComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void MeshComponent::Save(Json::Value& data) const
    {
    }

    void MeshComponent::Subscribe()
    {
        if (m_space != nullptr && m_mesh != nullptr)
        {
            m_space->GetScene()->AddMesh(m_mesh);
        }
    }

    void MeshComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_mesh != nullptr)
        {
            m_space->GetScene()->RemoveMesh(m_mesh);

        }
    }

    MeshComponent::MeshComponent(Object* owner)
        : Component(owner)
    {
    }

    void MeshComponent::Clone(MeshComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
        }
    }

    
}
