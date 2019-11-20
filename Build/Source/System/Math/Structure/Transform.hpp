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
        Vector3 GetPosition() const;
        Vector3 GetScale() const;
        Quaternion GetOrientation() const;

    private:
        Vector3 m_position;
        Vector3 m_scale;
        Quaternion m_orientation;
    };
}
