#include "RigidBodyComponent.hpp"

namespace Engine5
{
    RigidBodyComponent::RigidBodyComponent()
    {
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
    }

    void RigidBodyComponent::Initialize()
    {
        m_rigid_body->Initialize();
    }

    void RigidBodyComponent::Update(Real dt)
    {
        m_rigid_body->Update(dt);
    }

    void RigidBodyComponent::Shutdown()
    {
        m_rigid_body->Shutdown();
    }

    void RigidBodyComponent::ApplyForce(const Vector3& force, const Vector3& at) const
    {
        m_rigid_body->ApplyForce(force, at);
    }

    void RigidBodyComponent::ApplyForceCentroid(const Vector3& force) const
    {
        m_rigid_body->ApplyForceCentroid(force);
    }

    void RigidBodyComponent::SetLinearVelocity(const Vector3& linear) const
    {
        m_rigid_body->SetLinearVelocity(linear);
    }

    void RigidBodyComponent::SetAngularVelocity(const Vector3& angular) const
    {
        m_rigid_body->SetAngularVelocity(angular);
    }

    void RigidBodyComponent::SetMassInfinite() const
    {
        m_rigid_body->SetMassInfinite();
        m_rigid_body->SetInertiaInfinite();
    }

    void RigidBodyComponent::SetMass(Real mass) const
    {
        auto prev_inertia  = m_rigid_body->LocalInertia();
        auto prev_inv_mass = m_rigid_body->InverseMass();
        auto new_inertia   = (mass * prev_inv_mass) * prev_inertia;
        m_rigid_body->SetMass(mass);
        m_rigid_body->SetLocalInertia(new_inertia);
    }

    void RigidBodyComponent::SetPositionalConstraints(const Vector3& linear) const
    {
        m_rigid_body->SetPositionalConstraints(linear);
    }

    void RigidBodyComponent::SetRotationalConstraints(const Vector3& angular) const
    {
        m_rigid_body->SetRotationalConstraints(angular);
    }

    void RigidBodyComponent::SetMotionMode(MotionMode motion_mode) const
    {
        m_rigid_body->SetMotionMode(motion_mode);
    }

    Vector3 RigidBodyComponent::GetLinearVelocity() const
    {
        return m_rigid_body->GetLinearVelocity();
    }

    Vector3 RigidBodyComponent::GetAngularVelocity() const
    {
        return m_rigid_body->GetAngularVelocity();
    }

    Real RigidBodyComponent::GetMass() const
    {
        return m_rigid_body->Mass();
    }

    Real RigidBodyComponent::GetInverseMass() const
    {
        return m_rigid_body->InverseMass();
    }

    Matrix33 RigidBodyComponent::GetMassMatrix() const
    {
        return m_rigid_body->MassMatrix();
    }

    Matrix33 RigidBodyComponent::GetInverseMassMatrix() const
    {
        return m_rigid_body->InverseMassMatrix();
    }

    Matrix33 RigidBodyComponent::GetInertia() const
    {
        return m_rigid_body->Inertia();
    }

    Matrix33 RigidBodyComponent::GetInverseInertia() const
    {
        return m_rigid_body->InverseInertia();
    }

    MotionMode RigidBodyComponent::GetMotionMode() const
    {
        return m_rigid_body->GetMotionMode();
    }
}
