#pragma once
#include "../../Math/Math.hpp"
#include "..//Utility/PhysicsDef.hpp"
#include "..//ColliderPrimitive/ColliderPrimitive.hpp"
#include <vector>

namespace Engine5
{
    class ColliderPrimitive;
    class RigidBody
    {
    public:
        RigidBody();
        ~RigidBody();

        void Integrate(Real dt);

        void UpdateGlobalCentroidFromPosition();
        void UpdatePositionFromGlobalCentroid();
        void UpdateGlobalInertiaTensor();

        void UpdateOrientation();
        void UpdateMassData();

        void ApplyForce(const Vector3& force, const Vector3& at);
        void ApplyForceCentroid(const Vector3& force);

        void    SetLinearVelocity(const Vector3& linear);
        void    SetAngularVelocity(const Vector3& angular);
        Vector3 GetLinearVelocity() const;
        Vector3 GetAngularVelocity() const;

        void SetPositionalConstraints(const Vector3& linear);
        void SetRotationalConstraints(const Vector3& angular);

        void     SetMassInfinite();
        void     SetMass(Real mass = 1.0f);
        Real     Mass() const;
        Real     InverseMass() const;
        Matrix33 MassMatrix() const;
        Matrix33 InverseMassMatrix() const;

        void     SetInertiaInfinite();
        void     SetInertia(const Matrix33& inertia_tensor);
        Matrix33 Inertia() const;
        Matrix33 InverseInertia() const;

        void       SetMotionType(MotionMode motion_mode);
        MotionMode GetMotionType() const;

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;
    private:
        friend class Resolution;
        friend class ColliderPrimitive;

    private:
        //linear data
        Vector3 m_position; //position of rigid body
        Vector3 m_linear_velocity;
        Vector3 m_force_accumulator;
        Vector3 m_constraints_positional = Vector3(1.0f, 1.0f, 1.0f);

        //angular data
        Quaternion m_orientation;
        Quaternion m_inverse_orientation;
        Vector3    m_angular_velocity;
        Vector3    m_torque_accumulator;
        Vector3    m_constraints_rotational = Vector3(1.0f, 1.0f, 1.0f);

        //mass data
        Vector3  m_global_centroid; //center of mass
        Vector3  m_local_centroid;
        Real     m_mass         = 1.0f;
        Real     m_inverse_mass = 1.0f;
        Matrix33 m_local_inertia_tensor;
        Matrix33 m_local_inverse_inertia_tensor;
        Matrix33 m_global_inverse_inertia_tensor;

        //others
        MotionMode                       m_motion_mode        = MotionMode::Dynamic;
        std::vector<ColliderPrimitive*>* m_collider_list = nullptr;
    };
}
