#include "ControllerComponent.h"
#include "../../../System/Core/Input/InputCommon.hpp"
#include "../../../System/Core/Input/KeyboardInput.hpp"
#include "../../../Manager/Object/Object.hpp"
#include "../../../Manager/Component/EngineComponent/TransformComponent.hpp"
#include "../../../Manager/Space/Space.hpp"
#include "../../../System/Logic/LogicSubsystem.hpp"
#include "../../../Manager/Component/EngineComponent/CameraComponent.hpp"

namespace Game
{
    using namespace Engine5;

    ControllerComponent::~ControllerComponent()
    {
    }

    void ControllerComponent::Initialize()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->InitializeLogic(this);
        }
        Subscribe();
    }

    void ControllerComponent::Update(Real dt)
    {
        if (m_input != nullptr)
        {
            if (m_input->GetKeyboardInput()->IsDown(eKeyCodeKeyboard::A))
            {
                m_owner->GetComponent<TransformComponent>()->AddRotationY(-dt);
            }
            if (m_input->GetKeyboardInput()->IsDown(eKeyCodeKeyboard::D))
            {
                m_owner->GetComponent<TransformComponent>()->AddRotationY(dt);
            }
            if (m_input->GetKeyboardInput()->IsDown(eKeyCodeKeyboard::W))
            {
                m_owner->GetComponent<TransformComponent>()->AddRotationY(dt);
            }
            if (m_input->GetKeyboardInput()->IsDown(eKeyCodeKeyboard::S))
            {
                m_owner->GetComponent<TransformComponent>()->AddRotationY(dt);
            }
        }
    }

    void ControllerComponent::Shutdown()
    {
        Unsubscribe();
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
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->AddLogic(this);
        }
    }

    void ControllerComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->RemoveLogic(this);
        }
    }

    ControllerComponent::ControllerComponent(Object* owner)
        : LogicComponent(owner)
    {
    }

    void ControllerComponent::Clone(ControllerComponent* origin)
    {
    }
}
