#pragma once
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../DataType/Color.hpp"
#include "Light.hpp"

namespace Engine5
{
    class SpotLight final : public Light
    {
    public:
        SpotLight();
        ~SpotLight();

        void Initialize() override;

        void SetAmbientColor(const Color& color) override;
        void SetDiffuseColor(const Color& color) override;
        void SetSpecularColor(const Color& color) override;
        void SetSpecularPower(Real power) override;
        void SetDirection(const Vector3& dir) override;
        void SetPosition(const Vector3& pos) override;
        void SetAttenuation(const Vector3& att) override;
        void SetLength(Real len) override;
        void SetRange(Real rng) override;
        void SetSpot(Real s) override;

    public:

        Color   ambient_color;
        Color   diffuse_color;
        Color   specular_color;
        Vector3 position;
        Real    specular_power = 1.0f;
        Vector3 direction;
        Real    spot = 0.0f;
        Vector3 attenuation;
        Real    range = 1.0f;
    };
}
