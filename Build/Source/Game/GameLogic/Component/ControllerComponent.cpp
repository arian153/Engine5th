#include "ControllerComponent.h"
#include "../../../System/Core/Input/InputCommon.hpp"
#include "../../../System/Core/Input/KeyboardInput.hpp"
#include "../../../Manager/Object/Object.hpp"
#include "../../../Manager/Component/EngineComponent/TransformComponent.hpp"
#include "../../../Manager/Space/Space.hpp"
#include "../../../System/Logic/LogicSubsystem.hpp"
#include "../../../Manager/Component/EngineComponent/CameraComponent.hpp"
#include "../../../System/Core/Input/MouseInput.hpp"
#include "../../../System/Graphics/Utility/TextRenderer.hpp"

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
        if (m_input != nullptr && m_owner->HasComponent<CameraComponent>())
        {
            auto mouse    = m_input->GetMouseInput();
            auto keyboard = m_input->GetKeyboardInput();
            auto camera   = m_owner->GetComponent<CameraComponent>();
            int  curr_x   = mouse->CurrentPosition().x;
            int  curr_y   = mouse->CurrentPosition().y;
            int  prev_x   = mouse->PreviousPosition().x;
            int  prev_y   = mouse->PreviousPosition().y;
            Real dx       = static_cast<Real>(curr_x - prev_x);
            Real dy       = static_cast<Real>(curr_y - prev_y);
            if (mouse->IsDown(eKeyCodeMouse::Right))
            {
                camera->AddDistanceInUpDirection(-dt * dy);
                camera->AddDistanceInRightDirection(dt * dx);
            }
            if (m_input->GetMouseInput()->IsWheelRolling())
            {
                camera->AddDistanceInLookingDirection(dt * 20.0f * mouse->MouseWheelRollingDirection());
            }
            if (keyboard->IsDown(eKeyCodeKeyboard::W))
            {
                camera->AddRotationX(-dt);
            }
            if (keyboard->IsDown(eKeyCodeKeyboard::S))
            {
                camera->AddRotationX(dt);
            }
            if (keyboard->IsDown(eKeyCodeKeyboard::A))
            {
                camera->AddRotationY(-dt);
            }
            if (keyboard->IsDown(eKeyCodeKeyboard::D))
            {
                camera->AddRotationY(dt);
            }
            if (keyboard->IsDown(eKeyCodeKeyboard::Q))
            {
                camera->AddRotationZ(-dt);
            }
            if (keyboard->IsDown(eKeyCodeKeyboard::E))
            {
                camera->AddRotationZ(dt);
            }
        }
    }

    void ControllerComponent::Shutdown()
    {
        Unsubscribe();
    }

    void ControllerComponent::Render()
    {
        if (m_input != nullptr && m_owner->HasComponent<CameraComponent>())
        {
            auto camera = m_owner->GetComponent<CameraComponent>();
            auto basis  = camera->GetBasis();
            m_text_renderer->Output(
                                    Vector2(520), ColorDef::Pure::Red, "P : ", camera->GetPosition(),
                                    "\nI : ", basis.i, "\nJ : ", basis.j, "\nK : ", basis.k);
        }
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
