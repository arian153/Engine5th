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

        void SetAmbientColor(const Color& color);
        void SetDiffuseColor(const Color& color);
        void SetSpecularColor(const Color& color);
        void SetSpecularPower(Real power);
        void SetDirection(const Vector3& dir);
        void SetPosition(const Vector3& pos);
        void SetAttenuation(const Vector3& att);
        void SetLength(Real len);
        void SetRange(Real rng);
        void SetSpot(Real s);

        LightBufferData GetLightBuffer() const;
        void            GetLightBuffer(void* data) const;
    private:
        LightComponent* m_component = nullptr;
        eLightType      m_type      = eLightType::PointLight;
        Color           m_ambient_color;
        Color           m_ambient_range;
        Color           m_diffuse_color;
        Color           m_specular_color;
        Real            m_specular_power = 1.0f;

        Vector3 m_direction;
        Vector3 m_position;
        Real    m_range = 1.0f;
        Real    m_spot  = 0.0f;
        Vector3 m_attenuation;
        Real    m_length = 0.0f;
    };
}
