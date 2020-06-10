#include "ControllerComponent.h"
#include "../../../System/Core/Input/InputCommon.hpp"
#include "../../../System/Core/Input/KeyboardInput.hpp"
#include "../../../Manager/Object/Object.hpp"
#include "../../../Manager/Component/EngineComponent/TransformComponent.hpp"

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
        if (m_input->GetKeyboardInput()->IsPressed(eKeyCodeKeyboard::A))
        {
            m_owner->GetComponent<TransformComponent>()->AddRotationY(dt);
        }
    }

    void ControllerComponent::Shutdown()
    {
    }

    void ControllerComponent::Render()
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
