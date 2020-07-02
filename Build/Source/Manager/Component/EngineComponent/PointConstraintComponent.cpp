#include "PointConstraintComponent.hpp"

namespace Engine5
{
    PointConstraintComponent::~PointConstraintComponent()
    {
    }

    void PointConstraintComponent::Initialize()
    {
    }

    void PointConstraintComponent::Update(Real dt)
    {
    }

    void PointConstraintComponent::Shutdown()
    {
    }

    bool PointConstraintComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void PointConstraintComponent::Save(Json::Value& data) const
    {
    }

    void PointConstraintComponent::Subscribe()
    {
    }

    void PointConstraintComponent::Unsubscribe()
    {
    }

    PointConstraintComponent::PointConstraintComponent(Object* owner)
        : Component(owner)
    {
    }

    void PointConstraintComponent::Clone(PointConstraintComponent* origin)
    {
    }
}
