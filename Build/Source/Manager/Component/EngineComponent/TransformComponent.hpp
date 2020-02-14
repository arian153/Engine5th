#pragma once
#include "../Component.hpp"
#include "../../../System/Math/Math.hpp"
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class TransformFactory final : public ComponentFactory
    {
        TransformFactory();
        ~TransformFactory();

        Component* Create(Object* owner) override;
        Component* Clone(Component* origin, Object* dest) override;
    };

    class TransformComponent final : public Component
    {
    public:
        ~TransformComponent();
        TransformComponent() = delete;
        TransformComponent(const TransformComponent& rhs) = delete;
        TransformComponent& operator=(const TransformComponent& rhs) = delete;


        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        
        //Setter
        void SetPosition(const Vector3& position);
        void SetPosition(Real x, Real y, Real z);
        void SetOrientation(const Matrix33& rotation_matrix);
        void SetOrientation(const Quaternion& quaternion);
        void SetOrientation(const AxisRadian& axis_radian);
        void SetOrientation(const EulerAngle& rotation);
        void SetOrientationAxis(const Vector3& axis);
        void SetScale(const Vector3& scale, eAnchor3D anchor = eAnchor3D::CCC);
        void SetScale(Real x, Real y, Real z, eAnchor3D anchor = eAnchor3D::CCC);

        //Adder
        void AddPosition(const Vector3& delta_position);
        void AddRotation(const Quaternion& delta_rotation);
        void AddRotation(const AxisRadian& axis_radian);
        void AddRotationX(Real radian);
        void AddRotationY(Real radian);
        void AddRotationZ(Real radian);
        void AddRotationA(Real radian);
        void AddScale(const Vector3& delta_scale, eAnchor3D anchor = eAnchor3D::CCC);

        //Getter
        Vector3 GetPosition() const;
        Vector3 GetScale() const;
        Quaternion GetOrientation() const;
        Matrix33 GetRotationMatrix() const;
        Matrix44 GetTransformMatrix() const;

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

    protected:
        void Load() override;
        void Unload() override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class TransformFactory;

    private:
        explicit TransformComponent(Object* owner);
        void     Clone(TransformComponent* cloned);

        void UpdateChildrenPositionRecursive(const Vector3& position);
        void UpdateChildrenOrientationRecursive(const Quaternion& orientation);


    private:
        Transform  m_transform;
        AxisRadian m_axis_holder;
    };
}
