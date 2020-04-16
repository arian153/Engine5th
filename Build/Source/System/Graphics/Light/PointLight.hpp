#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../DataType/Color.hpp"
#include "Light.hpp"

namespace Engine5
{
    class PointLight final : public Light
    {
    public:
        PointLight();
        ~PointLight();

        void Initialize() override;

    public:
        Color   ambient_color;
        Color   diffuse_color;
        Color   specular_color;
        Vector3 position;
        Real    range = 1.0f;
        Vector3 attenuation;
        Real    padding = 0.0f;
    };
}
