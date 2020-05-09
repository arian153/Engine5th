#pragma once
#include <directxmath.h>

namespace Engine5
{
    class InstanceDataDX11
    {
    public:
        InstanceDataDX11();
        ~InstanceDataDX11();

    public:
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 scale;
        DirectX::XMFLOAT4 orientation;
        DirectX::XMFLOAT4 color;
    };
}
