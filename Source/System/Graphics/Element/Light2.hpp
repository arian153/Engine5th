#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../Common/Buffer2/ConstantBufferData.hpp"
#include "../DataType/Color.hpp"
#include "../Common/Light/LightDef.hpp"

namespace Engine5
{
    class LightComponent;

    class Light2
    {
    public:
        Light2();
        ~Light2();

        void Initialize();
        void Shutdown() const;

        LightBufferData GetLightBuffer() const;
        void            GetLightBuffer(void* data) const;

        void SetType(eLightType type);

    public:
        Color ambient_color;
        Color ambient_range;
        Color diffuse_color;
        Color specular_color;
        Real  specular_power = 1.0f;

        Vector3 direction;
        Vector3 position;
        Real    range = 1.0f;
        Real    spot  = 0.0f;
        Vector3 attenuation;
        Real    length = 0.0f;

    private:
        LightComponent* m_component = nullptr;
        eLightType      m_type      = eLightType::PointLight;
    };
}
