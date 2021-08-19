#include "EnvironmentMapComponent.hpp"

namespace Engine5
{
    EnvironmentMapComponent::~EnvironmentMapComponent()
    {
    }

    void EnvironmentMapComponent::Initialize()
    {
    }

    void EnvironmentMapComponent::Update(Real dt)
    {
    }

    void EnvironmentMapComponent::Shutdown()
    {
    }

    bool EnvironmentMapComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void EnvironmentMapComponent::Save(Json::Value& data) const
    {
    }

    void EnvironmentMapComponent::Edit(CommandRegistry* command_registry)
    {
    }

    void EnvironmentMapComponent::Subscribe()
    {
    }

    void EnvironmentMapComponent::Unsubscribe()
    {
    }

    EnvironmentMapComponent::EnvironmentMapComponent(Object* owner)
        : Component(owner)
    {
    }

    void EnvironmentMapComponent::Clone(EnvironmentMapComponent* origin)
    {
    }
}
