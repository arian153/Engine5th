#pragma once
#include "MathDef.hpp"
#include "../Algebra/Vector2.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Vector4.hpp"

namespace Engine5
{
    namespace Math
    {
        namespace Vector
        {
            constexpr size_t X = 0;
            constexpr size_t Y = 1;
            constexpr size_t Z = 2;
            constexpr size_t W = 3;
        }

        namespace Vector2
        {
            const Engine5::Vector2 ORIGIN(0.0f, 0.0f);
            const Engine5::Vector2 X_AXIS(1.0f, 0.0f);
            const Engine5::Vector2 Y_AXIS(0.0f, 1.0f);
            const Engine5::Vector2 POSITIVE_MAX(Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX);
            const Engine5::Vector2 POSITIVE_MIN(Math::REAL_POSITIVE_MIN, Math::REAL_POSITIVE_MIN);
            const Engine5::Vector2 NEGATIVE_MAX(Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX);
            const Engine5::Vector2 NEGATIVE_MIN(Math::REAL_NEGATIVE_MIN, Math::REAL_NEGATIVE_MIN);
        }

        namespace Vector3
        {
            const Engine5::Vector3 ORIGIN(0.0f, 0.0f, 0.0f);
            const Engine5::Vector3 X_AXIS(1.0f, 0.0f, 0.0f);
            const Engine5::Vector3 Y_AXIS(0.0f, 1.0f, 0.0f);
            const Engine5::Vector3 Z_AXIS(0.0f, 0.0f, 1.0f);
            const Engine5::Vector3 POSITIVE_MAX(Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX);
            const Engine5::Vector3 POSITIVE_MIN(Math::REAL_POSITIVE_MIN, Math::REAL_POSITIVE_MIN, Math::REAL_POSITIVE_MIN);
            const Engine5::Vector3 NEGATIVE_MAX(Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX);
            const Engine5::Vector3 NEGATIVE_MIN(Math::REAL_NEGATIVE_MIN, Math::REAL_NEGATIVE_MIN, Math::REAL_NEGATIVE_MIN);
        }

        namespace Vector4
        {
            const Engine5::Vector4 ORIGIN(0.0f, 0.0f, 0.0f, 0.0f);
            const Engine5::Vector4 X_AXIS(1.0f, 0.0f, 0.0f, 0.0f);
            const Engine5::Vector4 Y_AXIS(0.0f, 1.0f, 0.0f, 0.0f);
            const Engine5::Vector4 Z_AXIS(0.0f, 0.0f, 1.0f, 0.0f);
            const Engine5::Vector4 W_AXIS(0.0f, 0.0f, 0.0f, 1.0f);
            const Engine5::Vector4 POSITIVE_MAX(Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX);
            const Engine5::Vector4 POSITIVE_MIN(Math::REAL_POSITIVE_MIN, Math::REAL_POSITIVE_MIN, Math::REAL_POSITIVE_MIN, Math::REAL_POSITIVE_MIN);
            const Engine5::Vector4 NEGATIVE_MAX(Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX);
            const Engine5::Vector4 NEGATIVE_MIN(Math::REAL_NEGATIVE_MIN, Math::REAL_NEGATIVE_MIN, Math::REAL_NEGATIVE_MIN, Math::REAL_NEGATIVE_MIN);
        }
    }
}
