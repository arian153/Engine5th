#include "CapsuleLight.hpp"
#include "../../Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    CapsuleLight::CapsuleLight()
    {
    }

    CapsuleLight::~CapsuleLight()
    {
    }

    void CapsuleLight::Initialize()
    {
    }

    void CapsuleLight::SetAmbientColor(const Color& color)
    {
        ambient_color = color;
    }

    void CapsuleLight::SetDiffuseColor(const Color& color)
    {
        diffuse_color = color;
    }

    void CapsuleLight::SetSpecularColor(const Color& color)
    {
        specular_color = color;
    }

    void CapsuleLight::SetSpecularPower(Real power)
    {
        specular_power = power;
    }

    void CapsuleLight::SetDirection(const Vector3& dir)
    {
        direction = dir;
    }

    void CapsuleLight::SetPosition(const Vector3& pos)
    {
        position = pos;
    }

    void CapsuleLight::SetAttenuation(const Vector3& att)
    {
        attenuation = att;
    }

    void CapsuleLight::SetLength(Real len)
    {
        length = len;
    }

    void CapsuleLight::SetRange(Real rng)
    {
        range = rng;
    }

    void CapsuleLight::SetSpot(Real s)
    {
        E5_UNUSED_PARAM(s);
    }
}
