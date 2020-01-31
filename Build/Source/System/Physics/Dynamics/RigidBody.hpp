#pragma once
#include "../../Math/Math.hpp"
#include "..//Utility/PhysicsDef.hpp"
#include "MassData.hpp"

namespace Engine5
{
    class World;

    class RigidBody
    {
    public:
        explicit RigidBody(World* world);
        ~RigidBody();

        void Integrate(Real dt);

        void UpdateGlobalCentroidFromPosition();
        void UpdatePositionFromGlobalCentroid();
        void UpdateGlobalInertiaTensor();

        void UpdateOrientation();
        void SetMassData(const MassData& mass_data);

        void ApplyForce(const Vector3& force, const Vector3& at);
        void ApplyForceCentroid(const Vector3& force);

        void SetPosition(const Vector3& position);
        void SetCentroid(const Vector3& centroid);
        void SetOrientation(const Quaternion& orientation);

        Vector3    GetPosition() const;
        Vector3    GetCentroid() const;
        Vector3    GetLocalCentroid() const;
        Quaternion GetOrientation() const;

        void SetLinearVelocity(const Vector3& linear);
        void SetAngularVelocity(const Vector3& angular);
        void AddLinearVelocity(const Vector3& linear_delta);
        void AddAngularVelocity(const Vector3& angular_delta);

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

        Matrix33 LocalInertia() const;
        Matrix33 InverseLocalInertia() const;
        void     SetLocalInertia(const Matrix33& inertia);

        void       SetMotionMode(MotionMode motion_mode);
        MotionMode GetMotionMode() const;

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

        void SyncToTransform(Transform* transform) const;
        void SyncFromTransform(Transform* transform);

    private:
        friend class Resolution;
        friend class ColliderPrimitive;
        friend class World;
        friend class ColliderSet;

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
        MassData m_mass_data;
        Vector3  m_global_centroid; //center of mass
        Matrix33 m_global_inverse_inertia_tensor;

        //others
        MotionMode m_motion_mode = MotionMode::Dynamic;
        World*     m_world       = nullptr;
    };
}
