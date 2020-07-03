#include "PointConstraintComponent.hpp"
#include "../../../System/Physics/Resolution/Constraints/PointConstraint.hpp"
#include "../../Space/Space.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "RigidBodyComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Resource/ResourceType/JsonResource.hpp"

namespace Engine5
{
    PointConstraintComponent::~PointConstraintComponent()
    {
    }

    void PointConstraintComponent::Initialize()
    {
        if (m_b_init == true)
            return;
        if (m_owner->HasComponent<RigidBodyComponent>())
        {
            auto body    = m_owner->GetComponent<RigidBodyComponent>();
            m_rigid_body = body->GetRigidBody();
            m_b_init     = true;
            if (m_point_constraint == nullptr)
            {
                World* world                    = m_space->GetWorld();
                m_point_constraint              = new PointConstraint(m_rigid_body, world->GetConstraintUtility());
                m_point_constraint->m_component = this;
                Subscribe();
                m_b_init = true;
            }
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }
    }

    void PointConstraintComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void PointConstraintComponent::Shutdown()
    {
        Unsubscribe();
        if (m_point_constraint != nullptr)
        {
            m_point_constraint->Shutdown();
            delete m_point_constraint;
            m_point_constraint = nullptr;
        }
    }

    void PointConstraintComponent::SetConstraintMode(eConstraintMode mode) const
    {
        m_point_constraint->SetConstraintMode(mode);
    }

    void PointConstraintComponent::SetFrequency(Real frequency) const
    {
        m_point_constraint->SetFrequency(frequency);
    }

    void PointConstraintComponent::SetDampingRatio(Real damping_ratio) const
    {
        m_point_constraint->SetDampingRatio(damping_ratio);
    }

    void PointConstraintComponent::EnableRotation(bool b_rotation) const
    {
        m_point_constraint->EnableRotation(b_rotation);
    }

    void PointConstraintComponent::SetTargetPoint(const Vector3& target) const
    {
        m_point_constraint->SetTargetPoint(target);
    }

    void PointConstraintComponent::SetAnchorPoint(const Vector3& local_anchor) const
    {
        m_point_constraint->SetAnchorPoint(local_anchor);
    }

    bool PointConstraintComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Mode") && data["Damping Ratio"].isString())
        {
            std::string mode = data["Mode"].asString();
            if (mode == "Hard")
            {
                m_point_constraint->SetConstraintMode(eConstraintMode::Hard);
            }
            else if (mode == "Soft")
            {
                m_point_constraint->SetConstraintMode(eConstraintMode::Soft);
            }
            else
            {
                m_point_constraint->SetConstraintMode(eConstraintMode::Hard);
            }
        }
        if (JsonResource::HasMember(data, "Anchor") && JsonResource::IsVector3(data["Anchor"]))
        {
            m_point_constraint->SetAnchorPoint(JsonResource::AsVector3(data["Anchor"]));
        }
        if (JsonResource::HasMember(data, "Target") && JsonResource::IsVector3(data["Target"]))
        {
            m_point_constraint->SetTargetPoint(JsonResource::AsVector3(data["Target"]));
        }
        if (JsonResource::HasMember(data, "Damping Ratio") && data["Damping Ratio"].isDouble())
        {
            m_point_constraint->SetDampingRatio(data["Damping Ratio"].asFloat());
        }
        if (JsonResource::HasMember(data, "Frequency") && data["Frequency"].isDouble())
        {
            m_point_constraint->SetFrequency(data["Frequency"].asFloat());
        }
        if (JsonResource::HasMember(data, "Enable Rotation") && data["Enable Rotation"].isBool())
        {
            m_point_constraint->EnableRotation(data["Enable Rotation"].asBool());
        }
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
