#include "RigidBody.hpp"
#include "World.hpp"
#include "../../../Manager/Component/EngineComponent/RigidBodyComponent.hpp"

namespace Engine5
{
    RigidBody::RigidBody()
    {
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::Shutdown() const
    {
        //disconnect rigid body
        if (m_component != nullptr)
        {
            m_component->m_rigid_body = nullptr;
        }
    }

    void RigidBody::IntegrateVelocity(Real dt)
    {
        // integrate linear velocity
        m_linear_velocity += m_mass_data.inverse_mass * m_force_accumulator * dt;
        // integrate angular velocity
        m_angular_velocity += m_global_inverse_inertia_tensor * m_torque_accumulator * dt;
        // zero out accumulated force and torque
        m_force_accumulator.SetZero();
        m_torque_accumulator.SetZero();
    }

    void RigidBody::IntegratePosition(Real dt)
    {
        SyncFromTransform(m_transform);
        Vector3 delta_linear_velocity = m_linear_velocity * dt;
        delta_linear_velocity         = delta_linear_velocity.HadamardProduct(m_constraints_positional);
        m_global_centroid += delta_linear_velocity;
        // integrate orientation
        //Vector3 delta_angular_velocity = m_angular_velocity;
        Vector3 delta_angular_velocity(
                                       Math::DegreesToRadians(m_angular_velocity.x),
                                       Math::DegreesToRadians(m_angular_velocity.y),
                                       Math::DegreesToRadians(m_angular_velocity.z));
        delta_angular_velocity = delta_angular_velocity.HadamardProduct(m_constraints_rotational);
        Vector3 axis           = delta_angular_velocity.Unit();
        Real    radian         = delta_angular_velocity.Length() * dt;
        m_orientation.AddRotation(axis, radian);
        // update physical properties
        UpdateOrientation();
        UpdatePositionFromGlobalCentroid();
        SyncToTransform(m_transform);
    }

    void RigidBody::UpdateGlobalCentroidFromPosition()
    {
        m_global_centroid = m_orientation.Rotate(m_mass_data.local_centroid) + m_position;
    }

    void RigidBody::UpdatePositionFromGlobalCentroid()
    {
        m_position = m_orientation.Rotate(-m_mass_data.local_centroid) + m_global_centroid;
    }

    void RigidBody::UpdateGlobalInertiaTensor()
    {
        m_global_inverse_inertia_tensor = m_orientation * m_mass_data.local_inverse_inertia_tensor * m_inverse_orientation;
        m_global_inertia_tensor         = m_orientation * m_mass_data.local_inertia_tensor * m_inverse_orientation;
    }

    void RigidBody::UpdateOrientation()
    {
        m_orientation.SetNormalize();
        m_inverse_orientation = m_orientation.Inverse();
        m_inverse_orientation.SetNormalize();
    }

    void RigidBody::SetMassData(const MassData& mass_data)
    {
        m_mass_data = mass_data;
        if (m_motion_mode != eMotionMode::Dynamic)
        {
            SetMassInfinite();
            SetInertiaInfinite();
        }
    }

    void RigidBody::ApplyForce(const Vector3& force, const Vector3& at)
    {
        m_force_accumulator += force;
        m_torque_accumulator += (at - m_global_centroid).CrossProduct(force);
    }

    void RigidBody::ApplyForceCentroid(const Vector3& force)
    {
        m_force_accumulator += force;
    }

    void RigidBody::SetPosition(const Vector3& position)
    {
        m_position = position;
        UpdateGlobalCentroidFromPosition();
    }

    void RigidBody::SetCentroid(const Vector3& centroid)
    {
        m_global_centroid = centroid;
        UpdatePositionFromGlobalCentroid();
    }

    void RigidBody::SetOrientation(const Quaternion& orientation)
    {
        m_orientation = orientation;
        UpdateOrientation();
    }

    Vector3 RigidBody::GetPosition() const
    {
        return m_position;
    }

    Vector3 RigidBody::GetCentroid() const
    {
        return m_global_centroid;
    }

    Vector3 RigidBody::GetLocalCentroid() const
    {
        return m_mass_data.local_centroid;
    }

    Quaternion RigidBody::GetOrientation() const
    {
        return m_orientation;
    }

    void RigidBody::SetLinearVelocity(const Vector3& linear)
    {
        m_linear_velocity = linear;
    }

    void RigidBody::SetAngularVelocity(const Vector3& angular)
    {
        m_angular_velocity = angular;
    }

    void RigidBody::AddLinearVelocity(const Vector3& linear_delta)
    {
        m_linear_velocity += linear_delta;
    }

    void RigidBody::AddAngularVelocity(const Vector3& angular_delta)
    {
        m_angular_velocity += angular_delta;
    }

    Vector3 RigidBody::GetLinearVelocity() const
    {
        return m_linear_velocity;
    }

    Vector3 RigidBody::GetAngularVelocity() const
    {
        return m_angular_velocity;
    }

    void RigidBody::SetPositionalConstraints(const Vector3& linear)
    {
        m_constraints_positional = linear;
    }

    void RigidBody::SetRotationalConstraints(const Vector3& angular)
    {
        m_constraints_rotational = angular;
    }

    void RigidBody::SetMassInfinite()
    {
        m_mass_data.mass         = 0.0f;
        m_mass_data.inverse_mass = 0.0f;
    }

    void RigidBody::SetMass(Real mass)
    {
        m_mass_data.mass         = mass;
        m_mass_data.inverse_mass = 1.0f / mass;
    }

    Real RigidBody::Mass() const
    {
        return m_mass_data.mass;
    }

    Real RigidBody::InverseMass() const
    {
        return m_mass_data.inverse_mass;
    }

    Matrix33 RigidBody::MassMatrix() const
    {
        Real     mass = m_mass_data.mass;
        Matrix33 mass_matrix;
        mass_matrix.SetDiagonal(mass, mass, mass);
        return mass_matrix;
    }

    Matrix33 RigidBody::InverseMassMatrix() const
    {
        Real     inv_mass = m_mass_data.inverse_mass;
        Matrix33 inverse_mass_matrix;
        inverse_mass_matrix.SetDiagonal(inv_mass, inv_mass, inv_mass);
        return inverse_mass_matrix;
    }

    void RigidBody::SetInertiaInfinite()
    {
        m_global_inverse_inertia_tensor.SetZero();
        m_mass_data.local_inertia_tensor.SetZero();
        m_mass_data.local_inverse_inertia_tensor.SetZero();
    }

    void RigidBody::SetInertia(const Matrix33& inertia_tensor)
    {
        m_global_inverse_inertia_tensor          = inertia_tensor.Inverse();
        m_mass_data.local_inverse_inertia_tensor = m_inverse_orientation * m_global_inverse_inertia_tensor * m_orientation;
        m_mass_data.local_inertia_tensor         = m_mass_data.local_inverse_inertia_tensor.Inverse();
    }

    Matrix33 RigidBody::Inertia() const
    {
        return m_global_inertia_tensor;
    }

    Matrix33 RigidBody::InverseInertia() const
    {
        return m_global_inverse_inertia_tensor;
    }

    Matrix33 RigidBody::LocalInertia() const
    {
        return m_mass_data.local_inertia_tensor;
    }

    Matrix33 RigidBody::InverseLocalInertia() const
    {
        return m_mass_data.local_inverse_inertia_tensor;
    }

    void RigidBody::SetLocalInertia(const Matrix33& inertia)
    {
        m_mass_data.local_inertia_tensor         = inertia;
        m_mass_data.local_inverse_inertia_tensor = inertia.Inverse();
        m_global_inverse_inertia_tensor          = m_orientation * m_mass_data.local_inverse_inertia_tensor * m_inverse_orientation;
    }

    void RigidBody::SetMotionMode(eMotionMode motion_mode)
    {
        m_motion_mode = motion_mode;
    }

    eMotionMode RigidBody::GetMotionMode() const
    {
        return m_motion_mode;
    }

    Vector3 RigidBody::LocalToWorldPoint(const Vector3& local_point) const
    {
        return m_orientation.Rotate(local_point) + m_position;
    }

    Vector3 RigidBody::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_inverse_orientation.Rotate(world_point - m_position);
    }

    Vector3 RigidBody::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_orientation.Rotate(local_vector);
    }

    Vector3 RigidBody::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_inverse_orientation.Rotate(world_vector);
    }

    void RigidBody::SyncToTransform(Transform* transform) const
    {
        if (transform != nullptr)
        {
            transform->orientation = m_orientation;
            transform->position    = m_position;
        }
    }

    void RigidBody::SyncFromTransform(Transform* transform)
    {
        if (transform != nullptr)
        {
            if (m_position != transform->position || m_orientation != transform->orientation)
            {
                m_position    = transform->position;
                m_orientation = transform->orientation;
                UpdateGlobalCentroidFromPosition();
                UpdateGlobalInertiaTensor();
            }
        }
    }

    void RigidBody::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void RigidBody::Clone(RigidBody* origin)
    {
        if (origin != this)
        {
            //linear data
            m_position               = origin->m_position;
            m_linear_velocity        = origin->m_linear_velocity;
            m_force_accumulator      = origin->m_force_accumulator;
            m_constraints_positional = origin->m_constraints_positional;
            //angular data
            m_orientation            = origin->m_orientation;
            m_inverse_orientation    = origin->m_inverse_orientation;
            m_angular_velocity       = origin->m_angular_velocity;
            m_torque_accumulator     = origin->m_torque_accumulator;
            m_constraints_rotational = origin->m_constraints_rotational;
            //mass data
            m_mass_data                     = origin->m_mass_data;
            m_global_centroid               = origin->m_global_centroid;
            m_global_inverse_inertia_tensor = origin->m_global_inverse_inertia_tensor;
            //others
            m_motion_mode = origin->m_motion_mode;
        }
    }
}
