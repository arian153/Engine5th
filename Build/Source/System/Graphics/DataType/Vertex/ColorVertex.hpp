#pragma once
#include "../../Renderer/APIMacro.hpp"
#include "../../Renderer/DX11/Vertex/ColorVertexDX11.hpp"

namespace Engine5
{
    class Vector3;
    class Color;

    class ColorVertex : public ColorVertexAPI
    {
    public:
        ColorVertex();
        ColorVertex(const Vector3& position, const Color& color);

        Vector3 GetPosition() const;
        Color   GetColor() const;

    public:
    };
}
