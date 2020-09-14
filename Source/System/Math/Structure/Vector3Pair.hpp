#pragma once
#include "../Algebra/Vector3.hpp"

namespace Engine5
{
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
