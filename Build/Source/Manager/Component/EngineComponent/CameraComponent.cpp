#include "CameraComponent.hpp"

namespace Engine5
{
    //factory code
    CameraFactory::CameraFactory()
    {
    }

    CameraFactory::~CameraFactory()
    {
    }

    Component* CameraFactory::Create(Object* owner)
    {
        return new CameraComponent(owner);
    }

    Component* CameraFactory::Clone(Component* origin, Object* dest)
    {
        auto source = static_cast<CameraComponent*>(origin);
        auto cloned = static_cast<CameraComponent*>(this->Create(dest));
        source->Clone(cloned);
        return cloned;
    }

    //camera code
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
