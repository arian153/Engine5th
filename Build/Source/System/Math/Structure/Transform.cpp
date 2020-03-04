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
        scaling.SetScale(scale, 1.0f);
        Matrix44 rotation;
        rotation.SetRotation(orientation);
        Matrix44 translation;
        translation.SetTranslation(position);
        Matrix44 result(scaling);
        result *= rotation;
        result *= translation;
        return result;
    }

    Vector3 Transform::LocalToWorldPoint(const Vector3& local_point) const
    {
        return orientation.Rotate(local_point) + position;
    }

    Vector3 Transform::WorldToLocalPoint(const Vector3& world_point) const
    {
        return orientation.Inverse().Rotate(world_point - position);
    }

    Vector3 Transform::LocalToWorldVector(const Vector3& local_vector) const
    {
        return orientation.Rotate(local_vector);
    }

    Vector3 Transform::WorldToLocalVector(const Vector3& world_vector) const
    {
        return orientation.Inverse().Rotate(world_vector);
    }
}
