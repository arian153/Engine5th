#include "RigidBody.hpp"
#include "ColliderSet.hpp"

namespace Engine5
{
    RigidBody::RigidBody()
    {
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::Initialize()
    {
    }

    void RigidBody::Update(Real dt)
    {
        Integrate(dt);
    }

    void RigidBody::Shutdown()
    {
    }

    void RigidBody::Integrate(Real dt)
    {
        // integrate linear velocity
        m_linear_velocity += m_inverse_mass * m_force_accumulator * dt;

        // integrate angular velocity
        m_angular_velocity += m_global_inverse_inertia_tensor * m_torque_accumulator * dt;

        // zero out accumulated force and torque
        m_force_accumulator.SetZero();
        m_torque_accumulator.SetZero();
        Vector3 delta_linear_velocity = m_linear_velocity * dt;
        delta_linear_velocity         = delta_linear_velocity.HadamardProduct(m_constraints_positional);
        m_global_centroid += delta_linear_velocity;

        // integrate orientation
        //Vector3 delta_angular_velocity = m_angular_velocity;
        Vector3 delta_angular_velocity(
                                       Utility::DegreesToRadians(m_angular_velocity.x),
                                       Utility::DegreesToRadians(m_angular_velocity.y),
                                       Utility::DegreesToRadians(m_angular_velocity.z));
        delta_angular_velocity = delta_angular_velocity.HadamardProduct(m_constraints_rotational);
        Vector3 axis           = delta_angular_velocity.Unit();
        Real    radian         = delta_angular_velocity.Length() * dt;
        m_orientation.AddRotation(axis, radian);

        // update physical properties
        UpdateOrientation();
        UpdatePositionFromGlobalCentroid();
    }

    void RigidBody::UpdateGlobalCentroidFromPosition()
    {
        m_global_centroid = m_orientation.Rotate(m_local_centroid) + m_position;
    }

    void RigidBody::UpdatePositionFromGlobalCentroid()
    {
        m_position = m_orientation.Rotate(-m_local_centroid) + m_global_centroid;
    }

    void RigidBody::UpdateGlobalInertiaTensor()
    {
        m_global_inverse_inertia_tensor = m_orientation * m_local_inertia_tensor * m_inverse_orientation;
    }

    void RigidBody::UpdateOrientation()
    {
        m_orientation.SetNormalize();
        m_inverse_orientation = m_orientation.Inverse();
        m_inverse_orientation.SetNormalize();
    }

    void RigidBody::UpdateMassData()
    {
        // reset local centroid & mass
        m_local_centroid.SetZero();
        m_mass = 0.0f;
        if (m_collider_set != nullptr && m_collider_set->m_colliders != nullptr)
        {
            // compute local centroid & mass
            for (auto& collider_data : *m_collider_set->m_colliders)
            {
                // accumulate mass
                m_mass += collider_data->m_mass;

                // accumulate weighted contribution
                m_local_centroid += collider_data->m_mass * collider_data->m_centroid;
            }

            // compute inverse mass
            if (Utility::IsZero(m_mass) == false)
            {
                m_inverse_mass = 1.0f / m_mass;
            }
            else
            {
                //infinite mass.
                m_inverse_mass = 0.0f;
            }

            // compute final local centroid
            m_local_centroid *= m_inverse_mass;

            // compute local inertia tensor
            m_local_inertia_tensor.SetZero();
            for (auto& collider_data : *m_collider_set->m_colliders)
            {
                Vector3  r       = m_local_centroid - collider_data->m_centroid;
                Real     r_dot_r = r.DotProduct(r);
                Matrix33 r_out_r = r.OuterProduct(r);

                // accumulate local inertia tensor contribution, using Parallel Axis Theorem
                m_local_inertia_tensor += collider_data->m_local_inertia_tensor + collider_data->m_mass * (r_dot_r * Matrix33::Identity() - r_out_r);
            }

            // compute inverse inertia tensor
            m_local_inverse_inertia_tensor = m_local_inertia_tensor.Inverse();
            if (m_motion_mode != MotionMode::Dynamic)
            {
                this->SetMassInfinite();
                this->SetInertiaInfinite();

                //set collider mass data infinite? or not?
            }
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

    void RigidBody::SetLinearVelocity(const Vector3& linear)
    {
        m_linear_velocity = linear;
    }

    void RigidBody::SetAngularVelocity(const Vector3& angular)
    {
        m_angular_velocity = angular;
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
        m_mass         = 0.0f;
        m_inverse_mass = 0.0f;
    }

    void RigidBody::SetMass(Real mass)
    {
        m_mass         = mass;
        m_inverse_mass = 1.0f / mass;
    }

    Real RigidBody::Mass() const
    {
        return m_mass;
    }

    Real RigidBody::InverseMass() const
    {
        return m_inverse_mass;
    }

    Matrix33 RigidBody::MassMatrix() const
    {
        Matrix33 mass_matrix;
        mass_matrix.SetDiagonal(m_mass, m_mass, m_mass);
        return mass_matrix;
    }

    Matrix33 RigidBody::InverseMassMatrix() const
    {
        Matrix33 inverse_mass_matrix;
        inverse_mass_matrix.SetDiagonal(m_inverse_mass, m_inverse_mass, m_inverse_mass);
        return inverse_mass_matrix;
    }

    void RigidBody::SetInertiaInfinite()
    {
        m_global_inverse_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetZero();
        m_local_inverse_inertia_tensor.SetZero();
    }

    void RigidBody::SetInertia(const Matrix33& inertia_tensor)
    {
        m_global_inverse_inertia_tensor = inertia_tensor.Inverse();
        m_local_inertia_tensor          = m_inverse_orientation.ToMatrix() * m_global_inverse_inertia_tensor * m_orientation.ToMatrix();
        m_local_inverse_inertia_tensor  = m_local_inertia_tensor.Inverse();
    }

    Matrix33 RigidBody::Inertia() const
    {
        return m_global_inverse_inertia_tensor.Inverse();
    }

    Matrix33 RigidBody::InverseInertia() const
    {
        return m_global_inverse_inertia_tensor;
    }

    void RigidBody::SetMotionType(MotionMode motion_mode)
    {
        m_motion_mode = motion_mode;
    }

    MotionMode RigidBody::GetMotionType() const
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
}
