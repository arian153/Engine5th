#pragma once

#include <d3d11.h>
#include <directxmath.h>

namespace Engine5
{
    struct ColorBufferType
    {
        DirectX::XMFLOAT4 color;
    };

    class ColorBufferDX11
    {
    public:
        ColorBufferDX11();
        ~ColorBufferDX11();

    private:
    };
}
