#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../DataType/Color.hpp"
#include "Light.hpp"

namespace Engine5
{
    class DirectionalLight final : public Light
    {
    public:
        DirectionalLight();
        ~DirectionalLight();

        void Initialize() override;
    public:
        Color   ambient_color;
        Color   diffuse_color;
        Color   specular_color;
        Vector3 direction;
        Real    specular_power = 1.0f;
    };
}
