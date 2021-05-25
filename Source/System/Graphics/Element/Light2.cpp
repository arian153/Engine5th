#include "Light2.hpp"

namespace Engine5
{
    Light2::Light2()
    {
    }

    Light2::~Light2()
    {
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
}
