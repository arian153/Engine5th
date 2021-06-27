#include "LightComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Graphics/Element/Light2.hpp"
#include "../../Object/Object.hpp"

namespace Engine5
{
    LightComponent::~LightComponent()
    {
    }

    void LightComponent::Initialize()
    {
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }
    }

    void LightComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void LightComponent::Shutdown()
    {
        Unsubscribe();
        if (m_light != nullptr)
        {
            m_light->Shutdown();
            delete m_light;
            m_light = nullptr;
        }
    }

    void LightComponent::SetLightType(eLightType type)
    {
        m_light_type = type;
        m_light->SetType(type);
    }

    Light2* LightComponent::GetLight() const
    {
        return m_light;
    }

    bool LightComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Type") && data["Type"].isString())
        {
            std::string light_type = data["Type"].asString();
            if (light_type == "DirectionalLight")
            {
                SetLightType(eLightType::DirectionalLight);
            }
            else if (light_type == "PointLight")
            {
                SetLightType(eLightType::PointLight);
            }
            else if (light_type == "SpotLight")
            {
                SetLightType(eLightType::SpotLight);
            }
            else if (light_type == "CapsuleLight")
            {
                SetLightType(eLightType::CapsuleLight);
            }
        }
        if (JsonResource::HasMember(data, "Ambient Color") && JsonResource::IsColor(data["Ambient Color"]))
        {
            m_light->ambient_color = JsonResource::AsColor(data["Ambient Color"]);
        }
        if (JsonResource::HasMember(data, "Ambient Range") && JsonResource::IsColor(data["Ambient Range"]))
        {
            m_light->ambient_range = JsonResource::AsColor(data["Ambient Range"]);
        }
        if (JsonResource::HasMember(data, "Diffuse Color") && JsonResource::IsColor(data["Diffuse Color"]))
        {
            m_light->diffuse_color = JsonResource::AsColor(data["Diffuse Color"]);
        }
        if (JsonResource::HasMember(data, "Specular Color") && JsonResource::IsColor(data["Specular Color"]))
        {
            m_light->specular_color = (JsonResource::AsColor(data["Specular Color"]));
        }
        if (JsonResource::HasMember(data, "Specular Power") && data["Specular Power"].isDouble())
        {
            m_light->specular_power = (data["Specular Power"].asFloat());
        }

        if (JsonResource::HasMember(data, "Direction") && JsonResource::IsVector3(data["Direction"]))
        {
            m_light->direction = (JsonResource::AsVector3(data["Direction"]));
        }
        if (JsonResource::HasMember(data, "Position") && JsonResource::IsVector3(data["Position"]))
        {
            m_light->position = (JsonResource::AsVector3(data["Position"]));
        }
        if (JsonResource::HasMember(data, "Attenuation") && JsonResource::IsVector3(data["Attenuation"]))
        {
            m_light->attenuation = (JsonResource::AsVector3(data["Attenuation"]));
        }
        if (JsonResource::HasMember(data, "Length") && data["Length"].isDouble())
        {
            m_light->length = (data["Length"].asFloat());
        }
        if (JsonResource::HasMember(data, "Range") && data["Range"].isDouble())
        {
            m_light->range = (data["Range"].asFloat());
        }
        if (JsonResource::HasMember(data, "Spot") && data["Spot"].isDouble())
        {
            m_light->spot = (data["Spot"].asFloat());
        }
        return true;
    }

    void LightComponent::Save(Json::Value& data) const
    {
    }

    void LightComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void LightComponent::Subscribe()
    {
        if (m_space != nullptr && m_light != nullptr)
        {
            m_space->GetScene()->AddLight(m_light);
        }
    }

    void LightComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_light != nullptr)
        {
            m_space->GetScene()->RemoveLight(m_light);
        }
    }

    LightComponent::LightComponent(Object* owner)
        : Component(owner)
    {
    }

    void LightComponent::Clone(LightComponent* origin)
    {
    }
}
