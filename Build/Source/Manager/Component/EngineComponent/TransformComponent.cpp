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
        UpdateChildrenPositionRecursive(m_transform.position);
    }

    void TransformComponent::SetPosition(Real x, Real y, Real z)
    {
        m_transform.position.Set(x, y, z);
        UpdateChildrenPositionRecursive(m_transform.position);
    }

    void TransformComponent::SetOrientation(const Matrix33& rotation_matrix)
    {
        m_transform.orientation = rotation_matrix.ToQuaternion();
        m_axis_holder           = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientation(const Quaternion& quaternion)
    {
        m_transform.orientation = quaternion;
        m_axis_holder           = quaternion.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientation(const AxisRadian& axis_radian)
    {
        m_axis_holder = axis_radian;
        m_transform.orientation.Set(axis_radian.axis, axis_radian.radian);
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientation(const EulerAngle& rotation)
    {
        m_transform.orientation.Set(rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientationAxis(const Vector3& axis)
    {
        m_axis_holder.axis = axis;
        m_transform.orientation.Set(m_axis_holder.axis, m_axis_holder.radian);
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetScale(const Vector3& scale, Anchor3D anchor)
    {
        if (anchor == Anchor3D::CCC)
        {
            m_transform.scale = scale;
        }
        else
        {
            auto curr_size = m_transform.scale.Half();
            auto new_size  = scale.Half();
            m_transform.position += (new_size - curr_size).HadamardProduct(Anchor::AnchorVector3(anchor));
            m_transform.scale = scale;
        }
    }

    void TransformComponent::SetScale(Real x, Real y, Real z, Anchor3D anchor)
    {
        Vector3 scale(x, y, z);
        if (anchor == Anchor3D::CCC)
        {
            m_transform.scale = scale;
        }
        else
        {
            auto curr_size = m_transform.scale.Half();
            auto new_size  = scale.Half();
            m_transform.position += (new_size - curr_size).HadamardProduct(Anchor::AnchorVector3(anchor));
            m_transform.scale = scale;
        }
    }

    void TransformComponent::AddPosition(const Vector3& delta_position)
    {
        m_transform.position += delta_position;
        UpdateChildrenPositionRecursive(m_transform.position);
    }

    void TransformComponent::AddRotation(const Quaternion& delta_rotation)
    {
        m_transform.orientation.AddRotation(delta_rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotation(const AxisRadian& axis_radian)
    {
        m_transform.orientation.AddRotation(axis_radian.axis, axis_radian.radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationX(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::X_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationY(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::Y_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationZ(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::Z_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationA(Real radian)
    {
        m_axis_holder.radian += radian;
        m_transform.orientation.Set(m_axis_holder.axis, m_axis_holder.radian);
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddScale(const Vector3& delta_scale, Anchor3D anchor)
    {
        if (anchor == Anchor3D::CCC)
        {
            m_transform.scale += delta_scale;
        }
        else
        {
            auto curr_size = m_transform.scale.Half();
            auto new_size = (m_transform.scale + delta_scale).Half();
            m_transform.position += (new_size - curr_size).HadamardProduct(Anchor::AnchorVector3(anchor));
            m_transform.scale += delta_scale;
        }
    }

    void TransformComponent::UpdateChildrenPositionRecursive(const Vector3& position)
    {

    }

    void TransformComponent::UpdateChildrenOrientationRecursive(const Quaternion& orientation)
    {
    }
}
