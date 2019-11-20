#pragma once
#include "../Utility/MathDef.hpp"
#include "../Algebra/Vector3.hpp"

namespace Engine5
{
    class AxisRadian
    {
    public:
        explicit AxisRadian(const Vector3& axis = Vector3::AxisY(), Real radian = 0.0f);
        AxisRadian(const AxisRadian& rhs);
        ~AxisRadian();

        AxisRadian& operator=(const AxisRadian& rhs);

    public:
        Vector3 axis;
        Real    radian;
    };
}
