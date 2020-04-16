#include "LightComponent.hpp"
#include "../../../System/Graphics/Light/DirectionalLight.hpp"
#include "../../../System/Graphics/Light/PointLight.hpp"
#include "../../../System/Graphics/Light/SpotLight.hpp"
#include "../../../System/Graphics/Light/CapsuleLight.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    LightComponent::~LightComponent()
    {
    }

    void LightComponent::Initialize()
    {
        BuildLight();
    }

    void LightComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void LightComponent::Shutdown()
    {
        if (m_light != nullptr)
        {
            delete m_light;
            m_light = nullptr;
        }
    }

    void LightComponent::SetLightType(eLightType type)
    {
        if (m_light_type != type)
        {
            m_light_type = type;
            BuildLight();
        }
    }

    void LightComponent::BuildLight()
    {
        if (m_light != nullptr)
        {
            delete m_light;
            m_light = nullptr;
        }
        switch (m_light_type)
        {
        case Engine5::eLightType::DirectionalLight:
            m_light = new DirectionalLight();
            break;
        case Engine5::eLightType::PointLight:
            m_light = new PointLight();
            break;
        case Engine5::eLightType::SpotLight:
            m_light = new SpotLight();
            break;
        case Engine5::eLightType::CapsuleLight:
            m_light = new CapsuleLight();
            break;
        default:
            m_light = new DirectionalLight();
            break;
        }
        m_light->Initialize();
    }

    DirectionalLight* LightComponent::GetDirectionalLight() const
    {
        if (m_light_type == eLightType::DirectionalLight)
        {
            return static_cast<DirectionalLight*>(m_light);
        }
        return nullptr;
    }

    PointLight* LightComponent::GetPointLight() const
    {
        if (m_light_type == eLightType::PointLight)
        {
            return static_cast<PointLight*>(m_light);
        }
        return nullptr;
    }

    SpotLight* LightComponent::GetSpotLight() const
    {
        if (m_light_type == eLightType::SpotLight)
        {
            return static_cast<SpotLight*>(m_light);
        }
        return nullptr;
    }

    CapsuleLight* LightComponent::GetCapsuleLight() const
    {
        if (m_light_type == eLightType::CapsuleLight)
        {
            return static_cast<CapsuleLight*>(m_light);
        }
        return nullptr;
    }

    Light* LightComponent::GetLight() const
    {
        return m_light;
    }

    bool LightComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void LightComponent::Save(Json::Value& data) const
    {
    }

    void LightComponent::Subscribe()
    {
    }

    void LightComponent::Unsubscribe()
    {
    }

    LightComponent::LightComponent(Object* owner)
        : Component(owner)
    {
    }

    void LightComponent::Clone(LightComponent* origin)
    {
    }
}
