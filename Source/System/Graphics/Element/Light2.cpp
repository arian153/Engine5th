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

    void Light2::SetAmbientColor(const Color& color)
    {
        m_ambient_color = color;
    }

    void Light2::SetDiffuseColor(const Color& color)
    {
        m_diffuse_color = color;
    }

    void Light2::SetSpecularColor(const Color& color)

    {
        m_specular_color = color;
    }

    void Light2::SetSpecularPower(Real power)
    {
        m_specular_power = power;
    }

    void Light2::SetDirection(const Vector3& dir)
    {
        m_direction = dir;
    }

    void Light2::SetPosition(const Vector3& pos)
    {
        m_position = pos;
    }

    void Light2::SetAttenuation(const Vector3& att)
    {
        m_attenuation = att;
    }

    void Light2::SetLength(Real len)
    {
        m_length = len;
    }

    void Light2::SetRange(Real rng)
    {
        m_range = rng;
    }

    void Light2::SetSpot(Real s)
    {
        m_spot = s;
    }

    LightBufferData Light2::GetLightBuffer() const
    {
        LightBufferData result;
        result.ambient_color  = m_ambient_color;
        result.diffuse_color  = m_diffuse_color;
        result.specular_color = m_specular_color;
        result.specular_power = m_specular_power;
        result.direction      = m_direction;
        result.position       = m_position;
        result.range          = m_range;
        result.attenuation    = m_attenuation;
        result.length         = m_length;
        result.spot           = m_spot;
        result.type           = (int)m_type;
        return result;
    }

    void Light2::GetLightBuffer(void* data) const
    {
        LightBufferData* result = (LightBufferData*)data;
        result->ambient_color   = m_ambient_color;
        result->diffuse_color   = m_diffuse_color;
        result->specular_color  = m_specular_color;
        result->specular_power  = m_specular_power;
        result->direction       = m_direction;
        result->position        = m_position;
        result->range           = m_range;
        result->attenuation     = m_attenuation;
        result->length          = m_length;
        result->spot            = m_spot;
        result->type            = (int)m_type;
    }
}
