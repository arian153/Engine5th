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

    bool CameraComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void CameraComponent::Save(Json::Value& data) const
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
