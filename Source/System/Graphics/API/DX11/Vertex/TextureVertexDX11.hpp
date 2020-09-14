#pragma once
#include <directxmath.h>

namespace Engine5
{
    class TextureVertexDX11
    {
    public:
        TextureVertexDX11();
        ~TextureVertexDX11();

    public:
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };
}
