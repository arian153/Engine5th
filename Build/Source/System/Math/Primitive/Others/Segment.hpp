#pragma once
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"

namespace Engine5
{
    class Segment
    {
    public:
        Segment();
        ~Segment();

    public:
        Vector3 start;
        Vector3 end;
    };
}