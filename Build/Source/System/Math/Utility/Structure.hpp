#pragma once
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


    class EulerAngle
    {
    public:
        explicit EulerAngle(Real x = 0.0f, Real y = 0.0f, Real z = 0.0f);
        EulerAngle(const EulerAngle& rhs);
        ~EulerAngle();
        EulerAngle& operator=(const EulerAngle& rhs);

    public:
        Real x_rotation, y_rotation, z_rotation;
    };

    class Vector3Pair
    {
    public:
        explicit Vector3Pair(const Vector3& a = Vector3(), const Vector3& b = Vector3());
        Vector3Pair(const Vector3Pair& rhs);
        Vector3Pair& operator=(const Vector3Pair& rhs);
    public:
        Vector3 a;
        Vector3 b;
    };
}
