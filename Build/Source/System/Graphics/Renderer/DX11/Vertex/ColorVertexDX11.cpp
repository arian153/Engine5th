#include "../../../DataType/Vertex/ColorVertex.hpp"
#include "ColorVertexDX11.hpp"
#include "../../../../Math/Math.hpp"
#include "../../../DataType/Color.hpp"
#include "../Converter.hpp"

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
        position = Converter::ToXMFloat3(_position);
        color    = Converter::ToXMFloat4(_color);
    }
}
