#include "CameraComponent.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"

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
        if (m_space != nullptr)
        {
            m_space->GetScene();
        }
    }

    void CameraComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetScene();
        }
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
