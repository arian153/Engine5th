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
#include "../../../Manager/Component/EngineComponent/RigidBodyComponent.hpp"

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
            auto mouse    = m_input->GetMouseInput();
            auto keyboard = m_input->GetKeyboardInput();
            if (m_owner->HasComponent<CameraComponent>())
            {
                auto camera = m_owner->GetComponent<CameraComponent>();
                int  curr_x = mouse->CurrentPosition().x;
                int  curr_y = mouse->CurrentPosition().y;
                int  prev_x = mouse->PreviousPosition().x;
                int  prev_y = mouse->PreviousPosition().y;
                Real dx     = static_cast<Real>(curr_x - prev_x);
                Real dy     = static_cast<Real>(curr_y - prev_y);
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
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        camera->AddDistanceInUpDirection(dt * 10.0f);
                    }
                    else
                    {
                        camera->AddRotationX(-dt);
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::S))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        camera->AddDistanceInUpDirection(-dt * 10.0f);
                    }
                    else
                    {
                        camera->AddRotationX(dt);
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::A))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        camera->AddDistanceInRightDirection(-dt * 10.0f);
                    }
                    else
                    {
                        camera->AddRotationY(-dt);
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::D))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        camera->AddDistanceInRightDirection(dt * 10.0f);
                    }
                    else
                    {
                        camera->AddRotationY(dt);
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Q))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        camera->AddDistanceInLookingDirection(dt * 10.0f);
                    }
                    else
                    {
                        camera->AddRotationZ(-dt);
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::E))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        camera->AddDistanceInLookingDirection(-dt * 10.0f);
                    }
                    else
                    {
                        camera->AddRotationZ(dt);
                    }
                }
            }
            else
            {
                auto body = m_owner->GetComponent<RigidBodyComponent>();
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Left))
                {
                    body->ApplyForceCentroid(Vector3(-100.0f));
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Right))
                {
                    body->ApplyForceCentroid(Vector3(100.0f));
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Up))
                {
                    body->ApplyForceCentroid(Vector3(0.0f, 100.0f));
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Down))
                {
                    body->ApplyForceCentroid(Vector3(0.0f, -100.0f));
                }
            }
        }
    }

    void ControllerComponent::Shutdown()
    {
        Unsubscribe();
    }

    void ControllerComponent::Render()
    {
        if (m_owner->HasComponent<CameraComponent>())
        {
            auto camera = m_owner->GetComponent<CameraComponent>();
            auto basis  = camera->GetBasis();
            m_text_renderer->Output(
                                    Vector2(520), ColorDef::Pure::Red, "P : ", camera->GetPosition(),
                                    "\nI : ", basis.i, "\nJ : ", basis.j, "\nK : ", basis.k);
        }
        else
        {
            auto transform = m_owner->GetComponent<TransformComponent>();
            m_text_renderer->Output(
                                    Vector2(520, 400), ColorDef::Pure::Red, "O : ", transform->GetOrientation()
                                   );
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
