#pragma once
#include <directxmath.h>

namespace Engine5
{
    class Vector3;
    class Color;
    class ColorVertex
    {
    public:
        ColorVertex();
        ColorVertex(const Vector3& position, const Color& color);

    public:
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };
}
