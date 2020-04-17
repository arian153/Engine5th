#include "DirectionalLight.hpp"
#include "../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    DirectionalLight::DirectionalLight()
    {
    }

    DirectionalLight::~DirectionalLight()
    {
    }

    void DirectionalLight::Initialize()
    {
        ambient_color  = Color(0.15f, 0.15f, 0.15f);
        direction      = Vector3(0.0f, 0.0f, 1.0f);
        specular_power = 32.0f;
    }

    void DirectionalLight::SetAmbientColor(const Color& color)
    {
        ambient_color = color;
    }

    void DirectionalLight::SetDiffuseColor(const Color& color)
    {
        diffuse_color = color;
    }

    void DirectionalLight::SetSpecularColor(const Color& color)
    {
        specular_color = color;
    }

    void DirectionalLight::SetSpecularPower(Real power)
    {
        specular_power = power;
    }

    void DirectionalLight::SetDirection(const Vector3& dir)
    {
        direction = dir;
    }

    void DirectionalLight::SetPosition(const Vector3& pos)
    {
        E5_UNUSED_PARAM(pos);
    }

    void DirectionalLight::SetAttenuation(const Vector3& att)
    {
        E5_UNUSED_PARAM(att);
    }

    void DirectionalLight::SetLength(Real len)
    {
        E5_UNUSED_PARAM(len);
    }

    void DirectionalLight::SetRange(Real rng)
    {
        E5_UNUSED_PARAM(rng);
    }

    void DirectionalLight::SetSpot(Real s)
    {
        E5_UNUSED_PARAM(s);
    }
}
