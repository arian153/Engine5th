#include "Transform.hpp"

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
        Matrix44 scaling;
        scaling.SetTransformationScaling(scale, 1.0f);
        Matrix44 rotation;
        rotation.SetTransformationRotation(orientation);
        Matrix44 translation;
        translation.SetTransformationTranslation(position);
        Matrix44 result(scaling);
        result *= rotation;
        result *= translation;
        return result;
    }
}
