#include "Light2.hpp"

#include "../../../Manager/Component/EngineComponent/LightComponent.hpp"

namespace Engine5
{
    Light2::Light2()
    {
    }

    Light2::~Light2()
    {
    }

    void Light2::Initialize()
    {
    }

    void Light2::Update(Real dt) const
    {
        if (m_component != nullptr)
        {
            m_component->Update(dt);
        }
    }

    void Light2::Shutdown() const
    {
        if (m_component != nullptr)
        {
            m_component->m_light = nullptr;
        }
    }

    LightBufferData Light2::GetLightBuffer() const
    {
        return light_data;
    }

    void Light2::SetType(eLightType type)
    {
        m_type          = type;
        light_data.type = (int)type;
    }
}
