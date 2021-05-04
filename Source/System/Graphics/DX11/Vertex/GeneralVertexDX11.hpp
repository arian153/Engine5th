#pragma once
#include <directxmath.h>

namespace Engine5
{
    class GeneralVertexDX11
    {
    public:
        GeneralVertexDX11();
        ~GeneralVertexDX11();

    public:
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 tex;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT3 tangent;
    };
}
