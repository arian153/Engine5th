#include "Light.hpp"
#include "../../../Manager/Component/EngineComponent/LightComponent.hpp"

namespace Engine5
{
    Light::Light()
    {
    }

    Light::~Light()
    {
    }

    void Light::Shutdown() const
    {
        if (m_component != nullptr)
        {
            m_component->m_light = nullptr;
        }
    }
}
