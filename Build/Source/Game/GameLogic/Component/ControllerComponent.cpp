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
                Real dx     = Math::DegreesToRadians(0.25f * static_cast<Real>(curr_x - prev_x));
                Real dy     = Math::DegreesToRadians(0.25f * static_cast<Real>(curr_y - prev_y));
                if (mouse->IsDown(eKeyCodeMouse::Left))
                {
                    //camera->AddDistanceInUpDirection(-dt * dy);
                    //camera->AddDistanceInRightDirection(dt * dx);
                    m_theta += dx;
                    m_phi += dy;
                    // Restrict the angle phi.
                    m_phi = Math::Clamp(m_phi, 0.1f, Math::PI - 0.1f);
                    Vector3 eye_pos;
                    // Convert Spherical to Cartesian coordinates.
                    eye_pos.x = m_radius * sinf(m_phi) * cosf(m_theta);
                    eye_pos.z = m_radius * sinf(m_phi) * sinf(m_theta);
                    eye_pos.y = m_radius * cosf(m_phi);
                    camera->LookAt(target_pos + eye_pos, target_pos);
                }
                if (m_input->GetMouseInput()->IsWheelRolling())
                {
                    camera->AddDistanceInLookingDirection(dt * 20.0f * mouse->MouseWheelRollingDirection());
                    // Update angles based on input to orbit camera around box.
                    m_radius += 2.0f * mouse->MouseWheelRollingDirection();
                    // Restrict the radius.
                    m_radius = Math::Clamp(m_radius, 5.0f, 150.0f);
                    Vector3 eye_pos;
                    // Convert Spherical to Cartesian coordinates.
                    eye_pos.x = m_radius * sinf(m_phi) * cosf(m_theta);
                    eye_pos.z = m_radius * sinf(m_phi) * sinf(m_theta);
                    eye_pos.y = m_radius * cosf(m_phi);
                    camera->LookAt(target_pos + eye_pos, target_pos);
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
                auto body      = m_owner->GetComponent<RigidBodyComponent>();
                auto transform = m_owner->GetComponent<TransformComponent>();
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Left))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        transform->AddRotationZ(dt);
                    }
                    else
                    {
                        body->ApplyForceCentroid(Vector3(-10.0f));
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Right))
                {
                    if (keyboard->IsDown(eKeyCodeKeyboard::Space))
                    {
                        transform->AddRotationZ(-dt);
                    }
                    else
                    {
                        body->ApplyForceCentroid(Vector3(10.0f));
                    }
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Up))
                {
                    body->ApplyForceCentroid(Vector3(0.0f, 10.0f));
                }
                if (keyboard->IsDown(eKeyCodeKeyboard::Arrow_Down))
                {
                    body->ApplyForceCentroid(Vector3(0.0f, -10.0f));
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
        /*if (m_owner->HasComponent<CameraComponent>())
        {
            auto camera = m_owner->GetComponent<CameraComponent>();
            auto basis  = camera->GetBasis();
            m_text_renderer->Output(
                                    Vector2(520), ColorDef::Pure::Red, "P : ", camera->GetPosition(),
                                    "\nI : ", basis.i, "\nJ : ", basis.j, "\nK : ", basis.k);
        }
        else
        {
            auto       transform = m_owner->GetComponent<TransformComponent>();
            Transform* t_data    = transform->GetTransform();
            Vector3    test_data(-3, 4, 5);
            Matrix44   trans_mat = transform->GetTransformMatrix();
            Matrix44   inv       = trans_mat.Inverse();
            m_text_renderer->Output(
                                    Vector2(520, 400), ColorDef::Pure::Red,
                                    "Q : ", transform->GetOrientation(),
                                    "\nLocal To World\n",
                                    trans_mat.TransformVectorRotatingOrigin(test_data, transform->GetOrigin()),
                                    "\n",
                                    t_data->LocalToWorldRotatingOrigin(test_data),
                                    "\nWorld To Local\n",
                                    inv.TransformVectorRotatingOrigin(test_data, transform->GetOrigin()),
                                    "\n",
                                    t_data->WorldToLocalRotatingOrigin(test_data)
                                   );
        }*/
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
