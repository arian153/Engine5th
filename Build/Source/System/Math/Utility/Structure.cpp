#include "Structure.hpp"

namespace Engine5
{
    AxisRadian::AxisRadian(const Vector3& axis, Real radian)
        : axis(axis), radian(radian)
    {
    }

    AxisRadian::AxisRadian(const AxisRadian& rhs)
        : axis(rhs.axis), radian(rhs.radian)
    {
    }

    AxisRadian::~AxisRadian()
    {
    }

    AxisRadian& AxisRadian::operator=(const AxisRadian& rhs)
    {
        if (this != &rhs)
        {
            axis   = rhs.axis;
            radian = rhs.radian;
        }
        return *this;
    }

    EulerAngle::EulerAngle(Real x, Real y, Real z)
        : x_rotation(x), y_rotation(y), z_rotation(z)
    {
    }

    EulerAngle::EulerAngle(const EulerAngle& rhs)
        : x_rotation(rhs.x_rotation), y_rotation(rhs.y_rotation), z_rotation(rhs.z_rotation)
    {
    }

    EulerAngle::~EulerAngle()
    {
    }

    EulerAngle& EulerAngle::operator=(const EulerAngle& rhs)
    {
        if (this != &rhs)
        {
            x_rotation = rhs.x_rotation;
            y_rotation = rhs.y_rotation;
            z_rotation = rhs.z_rotation;
        }
        return *this;
    }

    Vector3Pair::Vector3Pair(const Vector3& a, const Vector3& b)
        : a(a), b(b)
    {
    }

    Vector3Pair::Vector3Pair(const Vector3Pair& rhs)
        : a(rhs.a), b(rhs.b)
    {
    }

    Vector3Pair& Vector3Pair::operator=(const Vector3Pair& rhs)
    {
        if (this != &rhs)
        {
            a = rhs.a;
            b = rhs.b;
        }
        return *this;
    }
}
