#pragma once
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Matrix44.hpp"

namespace Engine5
{
    class Transform
    {
    public:
        Transform();
        ~Transform();
        Matrix44 LocalToWorldMatrix() const;

    public:
        Vector3    position;
        Vector3    scale;
        Quaternion orientation;
    };
}
