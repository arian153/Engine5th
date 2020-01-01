#pragma once
#include "../Component.hpp"
#include "../../../System/Math/Math.hpp"

namespace Engine5
{
    class TransformComponent final : public Component
    {
    public:
        TransformComponent();
        ~TransformComponent();

        //Setter
        void SetPosition(const Vector3& position);
        void SetPosition(Real x, Real y, Real z);
        void SetOrientation(const Matrix33& rotation_matrix);
        void SetOrientation(const Quaternion& quaternion);
        void SetOrientation(const AxisRadian& axis_radian);
        void SetOrientation(const EulerAngle& rotation);
        void SetOrientationAxis(const Vector3& axis);
        void SetScale(const Vector3& scale, Anchor3D anchor = Anchor3D::CCC);
        void SetScale(Real x, Real y, Real z, Anchor3D anchor = Anchor3D::CCC);

        //Adder
        void AddPosition(const Vector3& delta_position);
        void AddRotation(const Quaternion& delta_rotation);
        void AddRotation(const AxisRadian& axis_radian);
        void AddRotationX(Real radian);
        void AddRotationY(Real radian);
        void AddRotationZ(Real radian);
        void AddRotationA(Real radian);
        void AddScale(const Vector3& delta_scale, Anchor3D anchor = Anchor3D::CCC);

    private:
        void UpdateChildrenPositionRecursive(const Vector3& position);
        void UpdateChildrenOrientationRecursive(const Vector3& orientation);
        void UpdateChildrenScaleRecursive(const Vector3& scale, const Vector3& position);

    private:
        Transform  m_transform;
        AxisRadian m_axis_holder;
    };
}
