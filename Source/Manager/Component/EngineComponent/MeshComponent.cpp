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
            m_mesh->AddInstance(m_transform, m_material_color);
        }
    }

    void MeshComponent::Shutdown()
    {
        Unsubscribe();
    }

    bool MeshComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Texture"))
        {
            auto texture_data = data["Texture"];

            if (JsonResource::HasMember(texture_data, "Diffuse") && texture_data["Diffuse"].isArray())
            {
                int idx = 0;
                for (auto it = texture_data["Diffuse"].begin(); it != texture_data["Diffuse"].end(); ++it)
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

            if (JsonResource::HasMember(texture_data, "Diffuse0"))
            {
                if (texture_data["Diffuse0"].isString())
                {
                    m_material_texture.diffuse0 = texture_data["Diffuse0"].asString();
                }
            }

            if (JsonResource::HasMember(texture_data, "Diffuse1"))
            {
                if (texture_data["Diffuse1"].isString())
                {
                    m_material_texture.diffuse0 = texture_data["Diffuse1"].asString();
                }
            }

            if (JsonResource::HasMember(texture_data, "Diffuse2"))
            {
                if (texture_data["Diffuse2"].isString())
                {
                    m_material_texture.diffuse0 = texture_data["Diffuse2"].asString();
                }
            }

            if (JsonResource::HasMember(texture_data, "DiffuseType"))
            {
                if (data["DiffuseType"].isString())
                {
                    std::string type = texture_data["DiffuseType"].asString();
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

            if (JsonResource::HasMember(texture_data, "Specular"))
            {
                if (texture_data["Specular"].isString())
                {
                    m_material_texture.specular0     = texture_data["Specular"].asString();
                    m_material_texture.specular_type = 1;
                }
                else
                {
                    m_material_texture.specular_type = 0;
                }
            }
            else
            {
                m_material_texture.specular_type = 0;
            }

            if (JsonResource::HasMember(texture_data, "NormalMap"))
            {
                if (texture_data["NormalMap"].isString())
                {
                    m_material_texture.normal0     = texture_data["NormalMap"].asString();
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

        if (JsonResource::HasMember(data, "Color"))
        {
            auto color_data = data["Color"];

            if (JsonResource::IsColor(color_data["Ambient"]))
            {
                m_material_color.ambient = JsonResource::AsColor(color_data["Ambient"]);
            }

            if (JsonResource::IsColor(color_data["Diffuse"]))
            {
                m_material_color.diffuse = JsonResource::AsColor(color_data["Diffuse"]);
            }

            if (JsonResource::IsColor(color_data["Specular"]))
            {
                m_material_color.specular = JsonResource::AsColor(color_data["Specular"]);
            }
        }

        if (JsonResource::HasMember(data, "Mesh"))
        {
            auto        mesh_data = data["Mesh"];
            std::string mesh_type;
            if (JsonResource::HasMember(mesh_data, "Type"))
            {
                mesh_type = mesh_data["Type"].asString();
            }

            if (mesh_data["Path"].isString() && mesh_type == "File")
            {
                m_model_resource_path = mesh_data["Path"].asString();
                m_model_resource      = m_space->GetResourceManager()->GetMeshResource(ToWString(m_model_resource_path));
                m_model_id            = reinterpret_cast<size_t>(m_model_resource);
            }
            else if (mesh_type == "Generate")
            {
                //E5_TODO : Need to add generated mesh
                //
            }
            else
            {
                //default mesh
            }
            AddMesh();
            //E5_TODO : Need to init properly Mesh2. 
        }
        return true;
    }

    void MeshComponent::Save(Json::Value& data) const
    {
    }

    void MeshComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
        //E5_TODO : Need to add user interface for mesh component
        //E5_TODO : Select Model Data from file
        //E5_TODO : Select Generated Model Data
        //E5_TODO : Select Texture Material
        //E5_TODO : Select Color Material
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

    void MeshComponent::AddMesh()
    {
        auto scene = m_space->GetScene();
        if (m_model_resource != nullptr)
        {
            m_mesh = scene->AddMesh(m_model_resource, m_material_texture);
        }
        else if (m_model_resource_path != "")
        {
            m_mesh = scene->AddMesh(m_model_resource_path, m_material_texture);
        }
        else
        {
            //E5_TODO : Need to add Generated Mesh Creation
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
