#include "ColorVertexDX11.hpp"
#include "../../../../Math/Math.hpp"
#include "../../../DataType/Color.hpp"
#include "../ConverterDX11.hpp"
#include "../../../Vertex/ColorVertex.hpp"

namespace Engine5
{
    ColorVertexDX11::ColorVertexDX11()
        : position(), color()
    {
    }

    ColorVertexDX11::~ColorVertexDX11()
    {
    }

    ColorVertex::ColorVertex()
    {
        position.x = 0.0f;
        position.y = 0.0f;
        position.z = 0.0f;
        color.w    = 1.0f;
        color.x    = 1.0f;
        color.y    = 1.0f;
        color.z    = 1.0f;
    }

    ColorVertex::ColorVertex(const Vector3& _position, const Color& _color)
    {
        position = ConverterDX11::ToXMFloat3(_position);
        color    = ConverterDX11::ToXMFloat4(_color);
    }

    Vector3 ColorVertex::GetPosition() const
    {
        return ConverterDX11::ToVector3(position);
    }

    Color ColorVertex::GetColor() const
    {
        return ConverterDX11::ToColor(color);
    }
}
