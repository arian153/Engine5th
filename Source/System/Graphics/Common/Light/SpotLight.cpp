#include "SpotLight.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    SpotLight::SpotLight()
    {
    }

    SpotLight::~SpotLight()
    {
    }

    void SpotLight::Initialize()
    {
    }

    void SpotLight::SetAmbientColor(const Color& color)
    {
        ambient_color = color;
    }

    void SpotLight::SetDiffuseColor(const Color& color)
    {
        diffuse_color = color;
    }

    void SpotLight::SetSpecularColor(const Color& color)
    {
        specular_color = color;
    }

    void SpotLight::SetSpecularPower(Real power)
    {
        specular_power = power;
    }

    void SpotLight::SetDirection(const Vector3& dir)
    {
        direction = dir;
    }

    void SpotLight::SetPosition(const Vector3& pos)
    {
        position = pos;
    }

    void SpotLight::SetAttenuation(const Vector3& att)
    {
        attenuation = att;
    }

    void SpotLight::SetLength(Real len)
    {
        E5_UNUSED_PARAM(len);
    }

    void SpotLight::SetRange(Real rng)
    {
        range = rng;
    }

    void SpotLight::SetSpot(Real s)
    {
        spot = s;
    }
}
