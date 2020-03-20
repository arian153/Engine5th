#include "RigidBodyComponent.hpp"
#include "../../Space/Space.hpp"

namespace Engine5
{
    RigidBodyComponent::~RigidBodyComponent()
    {
    }

    void RigidBodyComponent::Initialize()
    {
    }

    void RigidBodyComponent::Update(Real dt)
    {
        if (m_transform != nullptr)
        {
            m_rigid_body->SyncFromTransform(m_transform);
        }
        m_rigid_body->Integrate(dt);
        if (m_transform != nullptr)
        {
            m_rigid_body->SyncToTransform(m_transform);
        }
    }

    void RigidBodyComponent::Shutdown()
    {
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

    void RigidBodyComponent::SetMotionMode(eMotionMode motion_mode) const
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

    eMotionMode RigidBodyComponent::GetMotionMode() const
    {
        return m_rigid_body->GetMotionMode();
    }

    bool RigidBodyComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void RigidBodyComponent::Save(Json::Value& data) const
    {
    }

    void RigidBodyComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetWorld()->AddRigidBody(m_rigid_body);
        }
    }

    void RigidBodyComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetWorld()->RemoveRigidBody(m_rigid_body);
        }
    }

    RigidBodyComponent::RigidBodyComponent(Object* owner)
        : Component(owner)
    {
    }

    void RigidBodyComponent::Clone(RigidBodyComponent* cloned)
    {
        if (cloned != nullptr && cloned != this)
        {
            //copy data
        }
    }
}
