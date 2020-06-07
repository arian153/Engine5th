#include "ControllerComponent.h"

namespace Game
{
    using namespace Engine5;

    ControllerComponent::~ControllerComponent()
    {
    }

    void ControllerComponent::Initialize()
    {
    }

    void ControllerComponent::Update(Real dt)
    {
    }

    void ControllerComponent::Shutdown()
    {
    }

    bool ControllerComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void ControllerComponent::Save(Json::Value& data) const
    {
    }

    void ControllerComponent::Subscribe()
    {
    }

    void ControllerComponent::Unsubscribe()
    {
    }

    ControllerComponent::ControllerComponent(Object* owner)
        : LogicComponent(owner)
    {

    }

    void ControllerComponent::Clone(ControllerComponent* origin)
    {
    }
}
