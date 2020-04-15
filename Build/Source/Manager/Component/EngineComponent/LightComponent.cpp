#include "LightComponent.hpp"

namespace Engine5
{
    LightComponent::~LightComponent()
    {
    }

    void LightComponent::Initialize()
    {
    }

    void LightComponent::Update(Real dt)
    {
    }

    void LightComponent::Shutdown()
    {
    }

    bool LightComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void LightComponent::Save(Json::Value& data) const
    {
    }

    void LightComponent::Subscribe()
    {
    }

    void LightComponent::Unsubscribe()
    {
    }

    LightComponent::LightComponent(Object* owner)
        : Component(owner)
    {
    }

    void LightComponent::Clone(LightComponent* origin)
    {
    }
}
