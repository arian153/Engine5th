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

    void Light2::Shutdown() const
    {
        if (m_component != nullptr)
        {
            m_component->m_light = nullptr;
        }
    }

    LightBufferData Light2::GetLightBuffer() const
    {
        LightBufferData result;
        result.ambient_color  = ambient_color;
        result.diffuse_color  = diffuse_color;
        result.specular_color = specular_color;
        result.intensity = specular_power;
        result.direction      = direction;
        result.position       = position;
        result.range          = range;
        result.attenuation    = attenuation;
        result.length         = length;
        result.spot           = spot;
        result.type           = (int)m_type;
        return result;
    }

    void Light2::GetLightBuffer(void* data) const
    {
        LightBufferData* result = (LightBufferData*)data;
        result->ambient_color   = ambient_color;
        result->diffuse_color   = diffuse_color;
        result->specular_color  = specular_color;
        result->intensity  = specular_power;
        result->direction       = direction;
        result->position        = position;
        result->range           = range;
        result->attenuation     = attenuation;
        result->length          = length;
        result->spot            = spot;
        result->type            = (int)m_type;
    }

    void Light2::SetType(eLightType type)
    {
        m_type = type;
    }
}
