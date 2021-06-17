#include "MeshComponent.hpp"
#include "../../Object/Object.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Graphics/Common/Element/Mesh.hpp"
#include "../../../System/Graphics/Element/Mesh2.hpp"
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
        Subscribe();
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }
    }

    void MeshComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);

        if (m_mesh != nullptr)
        {
            m_mesh->AddInstance(m_transform, m_material_color.diffuse);
        }
    }

    void MeshComponent::Shutdown()
    {
        Unsubscribe();
    }

    bool MeshComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Material"))
        {
            auto material    = data["Material"];
            bool has_texture = false;

            if (JsonResource::HasMember(material, "Diffuse") && material["Diffuse"].isArray())
            {
                int idx = 0;
                for (auto it = material["Diffuse"].begin(); it != material["Diffuse"].end(); ++it)
                {
                    if ((*it).isString())
                    {
                        if (idx == 0)
                        {
                            m_material_texture.diffuse0 = (*it).asString();
                        }
                        if (idx == 1)
                        {
                            m_material_texture.diffuse1 = (*it).asString();
                        }
                        if (idx == 2)
                        {
                            m_material_texture.diffuse2 = (*it).asString();
                        }
                    }
                    idx++;
                }
            }

            if (JsonResource::HasMember(material, "Diffuse0"))
            {
                if (material["Diffuse0"].isString())
                {
                    m_material_texture.diffuse0 = material["Diffuse0"].asString();
                    has_texture                 = true;
                }
                if (JsonResource::IsColor(material["Diffuse0"]))
                {
                    m_material_color.diffuse = JsonResource::AsColor(material["Diffuse0"]);
                }
            }

            if (JsonResource::HasMember(material, "Diffuse1"))
            {
                if (material["Diffuse1"].isString())
                {
                    m_material_texture.diffuse0 = material["Diffuse1"].asString();
                    has_texture                 = true;
                }
                if (JsonResource::IsColor(material["Diffuse1"]))
                {
                    m_material_color.diffuse = JsonResource::AsColor(material["Diffuse1"]);
                }
            }

            if (JsonResource::HasMember(material, "Diffuse2"))
            {
                if (material["Diffuse2"].isString())
                {
                    m_material_texture.diffuse0 = material["Diffuse2"].asString();
                    has_texture                 = true;
                }
                if (JsonResource::IsColor(material["Diffuse2"]))
                {
                    m_material_color.diffuse = JsonResource::AsColor(material["Diffuse2"]);
                }
            }

            if (JsonResource::HasMember(material, "DiffuseType"))
            {
                if (data["DiffuseType"].isString())
                {
                    std::string type = material["DiffuseType"].asString();
                    if (type == "Texture")
                    {
                        m_material_texture.diffuse_type = 0;
                    }
                    else if (type == "AlphaMapping")
                    {
                        m_material_texture.diffuse_type = 1;
                    }
                    else if (type == "LightMapping")
                    {
                        m_material_texture.diffuse_type = 2;
                    }
                    else if (type == "Multi-Texture")
                    {
                        m_material_texture.diffuse_type = 3;
                    }
                    else
                    {
                        m_material_texture.diffuse_type = -1;
                    }
                }
            }

            if (JsonResource::HasMember(material, "Specular"))
            {
                if (material["Specular"].isString())
                {
                    m_material_texture.specular0     = material["Specular"].asString();
                    m_material_texture.specular_type = 1;
                }
                if (JsonResource::IsColor(material["Specular"]))
                {
                    m_material_color.specular        = JsonResource::AsColor(material["Specular"]);
                    m_material_texture.specular_type = 0;
                }
            }
            else
            {
                m_material_texture.specular_type = 0;
            }

            if (JsonResource::HasMember(material, "NormalMap"))
            {
                if (material["NormalMap"].isString())
                {
                    m_material_texture.normal0     = material["NormalMap"].asString();
                    m_material_texture.normal_type = 1;
                }
                else
                {
                    m_material_texture.normal_type = 0;
                }
            }
            else
            {
                m_material_texture.normal_type = 0;
            }
        }

        /* if (JsonResource::HasMember(data, "Shader Type") && data["Shader Type"].isString())
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
         }*/
        //if (JsonResource::HasMember(data, "Mesh"))
        //{
        //    if (data["Mesh"].isString())
        //    {
        //        std::string mesh     = data["Mesh"].asString();
        //        auto        resource = m_space->GetResourceManager()->GetMeshResource(ToWString(mesh));
        //        m_mesh->SetMeshData(resource->GetMeshData());
        //    }
        //    else
        //    {
        //        //create mesh data from mesh generator
        //    }
        //}
        return true;
    }

    void MeshComponent::Save(Json::Value& data) const
    {
    }

    void MeshComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void MeshComponent::Subscribe()
    {
        if (m_space != nullptr && m_mesh != nullptr)
        {
            m_space->GetScene()->AddMesh(this);
        }
    }

    void MeshComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_mesh != nullptr)
        {
            m_space->GetScene()->RemoveMesh(this);
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
