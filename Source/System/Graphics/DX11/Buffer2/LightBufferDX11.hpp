#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct LightBufferType
    {
        DirectX::XMFLOAT4 ambient_color;
        DirectX::XMFLOAT4 diffuse_color;
        DirectX::XMFLOAT4 specular_color;
        Real              specular_power;
        DirectX::XMFLOAT3 light_direction;
    };

    class LightBufferDX11
    {
    public:
        LightBufferDX11();
        ~LightBufferDX11();

    private:
    };
}
