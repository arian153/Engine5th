#include "ColorVertex.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../Color.hpp"
#include "../../../Math/Math.hpp"

namespace Engine5
{
    ColorVertex::ColorVertex()
    {
        position.x = 0.0f;
        position.y = 0.0f;
        position.z = 0.0f;
        color.w = 1.0f;
        color.x = 1.0f;
        color.y = 1.0f;
        color.z = 1.0f;
    }

    ColorVertex::ColorVertex(const Vector3& _position, const Color& _color)
    {
        position = Converter::ToXMFloat3(_position);
        color   = _color.ToXMFloat4();
    }
}
