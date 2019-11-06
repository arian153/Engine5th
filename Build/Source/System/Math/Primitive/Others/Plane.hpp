#pragma once
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"

namespace Engine5
{
    class Plane
    {
    public:
        Plane();
        ~Plane();

    public:
        Real a, b, c, d;
    };
}