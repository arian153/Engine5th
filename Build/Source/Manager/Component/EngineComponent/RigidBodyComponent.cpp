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
}
