#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct GammaBufferType
    {
        Real              gamma_correction;
        DirectX::XMFLOAT3 padding;
    };

    class GammaBufferDX11
    {
    public:
        GammaBufferDX11();
        ~GammaBufferDX11();

    protected:
    };
}
