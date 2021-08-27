#include "EnvironmentMapComponent.hpp"

#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Graphics/Element/SkyBox.hpp"
#include "../../../System/Graphics/Element/SkyDome.hpp"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../Resource/ResourceType/TextureResource.hpp"
#include "../../Space/Space.hpp"

namespace Engine5
{
    EnvironmentMapComponent::~EnvironmentMapComponent()
    {
    }

    void EnvironmentMapComponent::Initialize()
    {
        if (m_sky_type != -1)
        {
            if (m_sky_type == 0 && m_sky_box == nullptr)
            {
                m_sky_box = new SkyBox();
                m_sky_box->Initialize();
                m_sky_box->m_component = this;
                Subscribe();
            }

            if (m_sky_type == 1 && m_sky_dome == nullptr)
            {
                m_sky_dome = new SkyDome();
                m_sky_dome->Initialize();
                m_sky_dome->m_component = this;
                Subscribe();
            }
        }
    }

    void EnvironmentMapComponent::Update(Real dt)
    {
    }

    void EnvironmentMapComponent::Shutdown()
    {
        Unsubscribe();
        if (m_sky_box != nullptr)
        {
            m_sky_box->Shutdown();
            delete m_sky_box;
            m_sky_box = nullptr;
        }

        if (m_sky_dome != nullptr)
        {
            m_sky_dome->Shutdown();
            delete m_sky_dome;
            m_sky_dome = nullptr;
        }
    }

    bool EnvironmentMapComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Type"))
        {
            if (data["Type"].isString())
            {
                std::string sky_type = data["Type"].asString();

                if (sky_type == "CubeMap")
                {
                    m_sky_type = 0;
                }
                else if (sky_type == "Texture")
                {
                    m_sky_type = 1;
                }
            }

            if (data["Type"].isInt())
            {
                int sky_type = data["Type"].asInt();
                sky_type %= 2;
                m_sky_type = sky_type;
            }
        }

        if (JsonResource::HasMember(data, "Texture"))
        {
            if (data["Texture"].isString())
            {
                std::string texture_path = data["Texture"].asString();
                auto        resource     = m_space->GetResourceManager()->GetTextureResource(ToWString(texture_path));
                m_sky_texture            = resource->GetTexture();
            }
        }

        Initialize();

        return true;
    }

    void EnvironmentMapComponent::Save(Json::Value& data) const
    {
    }

    void EnvironmentMapComponent::Edit(CommandRegistry* command_registry)
    {
    }

    void EnvironmentMapComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            if (m_sky_box != nullptr)
            {
                m_space->GetScene()->AddSkyBox(m_sky_box);
            }

            if (m_sky_dome != nullptr)
            {
                m_space->GetScene()->AddSkyDome(m_sky_dome);
            }
        }
    }

    void EnvironmentMapComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            if (m_sky_box != nullptr)
            {
                m_space->GetScene()->RemoveSkyBox(m_sky_box);
            }

            if (m_sky_dome != nullptr)
            {
                m_space->GetScene()->RemoveSkyDome(m_sky_dome);
            }
        }
    }

    EnvironmentMapComponent::EnvironmentMapComponent(Object* owner)
        : Component(owner)
    {
    }

    void EnvironmentMapComponent::Clone(EnvironmentMapComponent* origin)
    {
    }

    void EnvironmentMapComponent::SwitchSkyType()
    {
        m_sky_type = (m_sky_type + 1) % 2;

        if (m_sky_box != nullptr)
        {
            Unsubscribe();
            m_sky_box->Shutdown();
            delete m_sky_box;
            m_sky_box  = nullptr;
            m_sky_dome = new SkyDome();
            m_sky_dome->Initialize();
            //E5_TODO : need to reselect texture.
            Subscribe();
        }
        else if (m_sky_dome != nullptr)
        {
            Unsubscribe();
            m_sky_dome->Shutdown();
            delete m_sky_dome;
            m_sky_dome = nullptr;
            m_sky_box  = new SkyBox();
            m_sky_box->Initialize();
            //E5_TODO : need to reselect texture.
            Subscribe();
        }
    }
}
