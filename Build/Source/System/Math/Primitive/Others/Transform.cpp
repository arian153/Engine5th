#include "Transform.h"

namespace Engine5
{
    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    Matrix44 Transform::LocalToWorldMatrix() const
    {
        return Matrix44();
    }

    Vector3 Transform::GetPosition() const
    {
        return m_position;
    }

    Vector3 Transform::GetScale() const
    {
        return m_scale;
    }

    Quaternion Transform::GetOrientation() const
    {
        return m_orientation;
    }
}
