#include "PointLight.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    PointLight::PointLight()
    {
    }

    PointLight::~PointLight()
    {
    }

    void PointLight::Initialize()
    {
    }

    void PointLight::SetAmbientColor(const Color& color)
    {
        ambient_color = color;
    }

    void PointLight::SetDiffuseColor(const Color& color)
    {
        diffuse_color = color;
    }

    void PointLight::SetSpecularColor(const Color& color)
    {
        specular_color = color;
    }

    void PointLight::SetSpecularPower(Real power)
    {
        specular_power = power;
    }

    void PointLight::SetDirection(const Vector3& dir)
    {
        E5_UNUSED_PARAM(dir);
    }

    void PointLight::SetPosition(const Vector3& pos)
    {
        position = pos;
    }

    void PointLight::SetAttenuation(const Vector3& att)
    {
        attenuation = att;
    }

    void PointLight::SetLength(Real len)
    {
        E5_UNUSED_PARAM(len);
    }

    void PointLight::SetRange(Real rng)
    {
        range = rng;
    }

    void PointLight::SetSpot(Real s)
    {
        E5_UNUSED_PARAM(s);
    }
}
