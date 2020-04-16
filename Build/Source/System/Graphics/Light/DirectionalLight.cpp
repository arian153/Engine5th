#include "DirectionalLight.hpp"

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
}
