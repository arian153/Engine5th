#include "CameraComponent.hpp"

namespace Engine5
{
    CameraComponent::~CameraComponent()
    {
    }

    void CameraComponent::Initialize()
    {
    }

    void CameraComponent::Update(Real dt)
    {
    }

    void CameraComponent::Shutdown()
    {
    }

    void CameraComponent::Load()
    {
    }

    void CameraComponent::Unload()
    {
    }

    void CameraComponent::Subscribe()
    {
    }

    void CameraComponent::Unsubscribe()
    {
    }

    CameraComponent::CameraComponent(Object* owner)
        : Component(owner)
    {
    }

    void CameraComponent::Clone(CameraComponent* cloned)
    {
        if (cloned != nullptr && cloned != this)
        {
            //copy data
        }
    }
}
