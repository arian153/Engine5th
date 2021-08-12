#include "RotatingComponent.h"

#include "../../../Manager/Component/EngineComponent/LightComponent.hpp"
#include "../../../Manager/Object/Object.hpp"
#include "../../../Manager/Component/EngineComponent/TransformComponent.hpp"
#include "../../../Manager/Space/Space.hpp"
#include "../../../System/Logic/LogicSubsystem.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"

namespace Game
{
    using namespace Engine5;

    RotatingComponent::~RotatingComponent()
    {
    }

    void RotatingComponent::Initialize()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->InitializeLogic(this);
        }
        Subscribe();

        s_basis.CalculateBasisQuaternion(s_normal);
    }

    void RotatingComponent::Update(Engine5::Real dt)
    {
        if (m_owner->HasComponent<Engine5::TransformComponent>() && s_b_pause == false)
        {
            m_elapsed_angle += s_ball_speed * dt;
            Vector3 position  = s_orbit_radius * (cosf(m_elapsed_angle) * s_basis.j + sinf(m_elapsed_angle) * s_basis.k);
            auto    transform = m_owner->GetComponent<Engine5::TransformComponent>();
            transform->SetPosition(s_orbit_center + position);

            if (m_owner->HasComponent<Engine5::LightComponent>())
            {
                auto light = m_owner->GetComponent<Engine5::LightComponent>();
                light->SetLightDirection((-position).Unit());
            }
        }
    }

    void RotatingComponent::Shutdown()
    {
        Unsubscribe();
    }

    void RotatingComponent::Render()
    {
    }

    bool RotatingComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void RotatingComponent::Save(Json::Value& data) const
    {
    }

    void RotatingComponent::Edit(Engine5::CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            std::string state = s_b_pause ? "Resume" : "Pause";

            ImGui::Checkbox(state.c_str(), &s_b_pause);
            ImGui::Text("Ball Speed");
            ImGui::SameLine();
            if (ImGui::Button("Zero##Ball Speed"))
            {
                s_ball_speed = 0.0f;
            }
            ImGui::SliderFloat("##Ball Speed", &s_ball_speed, -2.0f, 2.0f);
            ImGui::Text("Orbit Radius");
            ImGui::SliderFloat("##Orbit Radius", &s_orbit_radius, 0.0f, 20.0f);
            ImGui::Text("Orbit Center");
            ImGui::InputFloat3("##Orbit Center", &s_orbit_center.x);
            ImGui::Text("Orbit Normal");
            if (ImGui::InputFloat3("##Orbit Normal", &s_normal.x))
            {
                s_normal.SetNormalize();
                s_basis.CalculateBasisQuaternion(s_normal);
            }
        }
    }

    void RotatingComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->AddLogic(this);
        }
    }

    void RotatingComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->RemoveLogic(this);
        }
    }

    RotatingComponent::RotatingComponent(Engine5::Object* owner)
        : LogicComponent(owner)
    {
    }

    void RotatingComponent::Clone(RotatingComponent* origin)
    {
    }
}
