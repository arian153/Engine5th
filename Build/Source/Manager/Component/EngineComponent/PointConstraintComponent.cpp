#include "PointConstraintComponent.hpp"
#include "../../../System/Physics/Resolution/Constraints/PointConstraint.hpp"
#include "../../Space/Space.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "RigidBodyComponent.hpp"

namespace Engine5
{
    PointConstraintComponent::~PointConstraintComponent()
    {
    }

    void PointConstraintComponent::Initialize()
    {
        if (m_owner->HasComponent<RigidBodyComponent>())
        {
            auto body    = m_owner->GetComponent<RigidBodyComponent>();
            m_rigid_body = body->GetRigidBody();
            m_b_init     = true;
            if (m_point_constraint == nullptr)
            {
                World* world       = m_space->GetWorld();
                m_point_constraint = new PointConstraint(m_rigid_body, world->GetConstraintUtility());
                Subscribe();
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
    }

    void PointConstraintComponent::Shutdown()
    {
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
