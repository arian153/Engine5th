#pragma once
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class LightComponent;
    class Color;
    class Vector3;

    class Light
    {
    public:
        Light();
        virtual ~Light();

        virtual void Initialize() = 0;
        void         Shutdown() const;

        virtual void SetAmbientColor(const Color& color) = 0;
        virtual void SetDiffuseColor(const Color& color) = 0;
        virtual void SetSpecularColor(const Color& color) = 0;
        virtual void SetSpecularPower(Real power) = 0;
        virtual void SetDirection(const Vector3& dir) = 0;
        virtual void SetPosition(const Vector3& pos) = 0;
        virtual void SetAttenuation(const Vector3& att) = 0;
        virtual void SetLength(Real len) = 0;
        virtual void SetRange(Real rng) = 0;
        virtual void SetSpot(Real s) = 0;

    private:
        friend class LightComponent;

    private:
        LightComponent* m_component = nullptr;
    };
}
