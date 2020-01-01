#include "TransformComponent.hpp"

namespace Engine5
{
    TransformComponent::TransformComponent()
    {
    }

    TransformComponent::~TransformComponent()
    {
    }

    void TransformComponent::SetPosition(const Vector3& position)
    {
        m_transform.position = position;
    }

    void TransformComponent::SetPosition(Real x, Real y, Real z)
    {
        m_transform.position.Set(x, y, z);
    }

    void TransformComponent::SetOrientation(const Matrix33& rotation_matrix)
    {
        m_transform.orientation = rotation_matrix.ToQuaternion();
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformComponent::SetOrientation(const Quaternion& quaternion)
    {
        m_transform.orientation = quaternion;
        m_axis_holder = quaternion.ToAxisRadian();
    }

    void TransformComponent::SetOrientation(const AxisRadian& axis_radian)
    {
        m_axis_holder = axis_radian;
        m_transform.orientation.Set(axis_radian.axis, axis_radian.radian);
    }

    void TransformComponent::SetOrientation(const EulerAngle& rotation)
    {
        m_transform.orientation.Set(rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformComponent::SetOrientationAxis(const Vector3& axis)
    {
        m_axis_holder.axis = axis;
        m_transform.orientation.Set(m_axis_holder.axis, m_axis_holder.radian);
    }

    void TransformComponent::SetScale(const Vector3& scale, Anchor3D anchor)
    {
    }

    void TransformComponent::SetScale(Real x, Real y, Real z, Anchor3D anchor)
    {
    }

    void TransformComponent::AddPosition(const Vector3& delta_position)
    {
    }

    void TransformComponent::AddRotation(const Quaternion& delta_rotation)
    {
    }

    void TransformComponent::AddRotation(const AxisRadian& axis_radian)
    {
    }

    void TransformComponent::AddRotationX(Real radian)
    {
    }

    void TransformComponent::AddRotationY(Real radian)
    {
    }

    void TransformComponent::AddRotationZ(Real radian)
    {
    }

    void TransformComponent::AddRotationA(Real radian)
    {
    }

    void TransformComponent::AddScale(const Vector3& delta_scale, Anchor3D anchor)
    {
    }

    void TransformComponent::UpdateChildrenPositionRecursive(const Vector3& position)
    {
    }

    void TransformComponent::UpdateChildrenOrientationRecursive(const Vector3& orientation)
    {
    }

    void TransformComponent::UpdateChildrenScaleRecursive(const Vector3& scale, const Vector3& position)
    {
    }
}
