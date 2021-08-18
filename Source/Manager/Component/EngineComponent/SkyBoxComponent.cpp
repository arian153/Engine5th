#include "SkyBoxComponent.hpp"

namespace Engine5
{
    SkyBoxComponent::~SkyBoxComponent()
    {
    }

    void SkyBoxComponent::Initialize()
    {
    }

    void SkyBoxComponent::Update(Real dt)
    {
    }

    void SkyBoxComponent::Shutdown()
    {
    }

    bool SkyBoxComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void SkyBoxComponent::Save(Json::Value& data) const
    {
    }

    void SkyBoxComponent::Edit(CommandRegistry* command_registry)
    {
    }

    void SkyBoxComponent::Subscribe()
    {
    }

    void SkyBoxComponent::Unsubscribe()
    {
    }

    SkyBoxComponent::SkyBoxComponent(Object* owner)
        : Component(owner)
    {
    }

    void SkyBoxComponent::Clone(SkyBoxComponent* origin)
    {
    }
}
