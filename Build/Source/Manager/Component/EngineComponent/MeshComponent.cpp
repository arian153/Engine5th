#include "MeshComponent.hpp"
#include "../../../System/Graphics/Element/Mesh.hpp"
#include "../../Object/Object.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/TextureResource.hpp"
#include "../../Resource/ResourceType/MeshResource.hpp"

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
        if (JsonResource::HasMember(data, "Color") && JsonResource::IsColor(data["Color"]))
        {
            m_mesh->SetColor(JsonResource::AsColor(data["Color"]));
        }
        if (JsonResource::HasMember(data, "Texture") && data["Texture"].isString())
        {
            std::string texture  = data["Texture"].asString();
            auto        resource = m_space->GetResourceManager()->GetTextureResource(ToWString(texture));
            m_mesh->SetTexture(resource->GetTexture());
        }
        if (JsonResource::HasMember(data, "Texture Array") && data["Texture Array"].isArray())
        {
            for (auto it = data["Texture Array"].begin(); it != data["Texture Array"].end(); ++it)
            {
                if ((*it).isString())
                {
                    std::string texture  = (*it).asString();
                    auto        resource = m_space->GetResourceManager()->GetTextureResource(ToWString(texture));
                    m_mesh->AddTexture(resource->GetTexture());
                }
            }
        }
        if (JsonResource::HasMember(data, "Shader Type") && data["Shader Type"].isString())
        {
            std::string shader = data["Shader Type"].asString();
            if (shader == "Color")
            {
                m_mesh->SetShaderType(eShaderType::Color);
            }
            else if (shader == "Texture")
            {
                m_mesh->SetShaderType(eShaderType::Texture);
            }
            else if (shader == "Forward Directional Light")
            {
                m_mesh->SetShaderType(eShaderType::ForwardDirectionalLight);
            }
            else if (shader == "Deferred Directional Light")
            {
                m_mesh->SetShaderType(eShaderType::DeferredDirectionalLight);
            }
            else if (shader == "Multi Texture")
            {
                m_mesh->SetShaderType(eShaderType::MultiTexture);
            }
            else if (shader == "Alpha Mapping")
            {
                m_mesh->SetShaderType(eShaderType::AlphaMapping);
            }
            else if (shader == "Light Mapping")
            {
                m_mesh->SetShaderType(eShaderType::LightMapping);
            }
            else if (shader == "Normal Mapping")
            {
                m_mesh->SetShaderType(eShaderType::NormalMapping);
            }
            else if (shader == "Specular Mapping")
            {
                m_mesh->SetShaderType(eShaderType::SpecularMapping);
            }
            if (m_space != nullptr && m_mesh != nullptr)
            {
                m_space->GetScene()->ChangeShaderType(m_mesh);
            }
        }
        if (JsonResource::HasMember(data, "Mesh"))
        {
            if (data["Mesh"].isString())
            {
                std::string mesh     = data["Mesh"].asString();
                auto        resource = m_space->GetResourceManager()->GetMeshResource(ToWString(mesh));
                m_mesh->SetMeshData(resource->GetMeshData());
            }
            else
            {
                //create mesh data from mesh generator
            }
        }
        return true;
    }

    void MeshComponent::Save(Json::Value& data) const
    {
    }

    void MeshComponent::Edit()
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
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
