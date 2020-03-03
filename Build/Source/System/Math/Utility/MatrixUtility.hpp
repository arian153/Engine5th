#pragma once
#include "MathDef.hpp"

namespace Engine5
{
    class AxisRadian;
    class Quaternion;
    class Matrix44;
    class Matrix33;
    class EulerAngle;
    class Vector3;
    class Vector4;

    namespace Math
    {
        namespace Matrix44
        {
            //rotation
            Engine5::Matrix44 Rotation(const Quaternion& quaternion);
            Engine5::Matrix44 Rotation(const Engine5::Matrix33& rotation_matrix);
            Engine5::Matrix44 Rotation(const EulerAngle& euler_angle);
            Engine5::Matrix44 Rotation(const AxisRadian& axis_radian);

            //principle rotation
            Engine5::Matrix44 RotationX(Real radian);
            Engine5::Matrix44 RotationY(Real radian);
            Engine5::Matrix44 RotationZ(Real radian);

            //scale
            Engine5::Matrix44 Scale(const Engine5::Vector4& scale);
            Engine5::Matrix44 Scale(const Engine5::Vector3& scale, Real w = 1.0f);

            //translation
            Engine5::Matrix44 Translation(const Engine5::Vector3& translation);
        }

        namespace Matrix33
        {
            //rotation
            Engine5::Matrix33 Rotation(const Quaternion& quaternion);
            Engine5::Matrix33 Rotation(const Engine5::Matrix44& rotation_matrix);
            Engine5::Matrix33 Rotation(const EulerAngle& euler_angle);
            Engine5::Matrix33 Rotation(const AxisRadian& axis_radian);

            //principle rotation
            Engine5::Matrix33 RotationX(Real radian);
            Engine5::Matrix33 RotationY(Real radian);
            Engine5::Matrix33 RotationZ(Real radian);

            //scale
            Engine5::Matrix33 Scale(const Engine5::Vector3& scale);
        }
    }
}
