#include "RigidBodyComponent.hpp"
#include "../../Space/Space.hpp"
#include "../../Object/Object.hpp"
#include "ColliderComponent.hpp"
#include "TransformComponent.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Core/Utility/CoreUtility.hpp"

namespace Engine5
{
    RigidBodyComponent::~RigidBodyComponent()
    {
    }

    void RigidBodyComponent::Initialize()
    {
        if (m_rigid_body == nullptr)
        {
            m_rigid_body = new RigidBody();
            m_rigid_body->m_component = this;
            Subscribe();
        }
        if (m_collider_set != nullptr)
        {
            m_b_init = true;
        }
        else
        {
            if (m_owner->HasComponent<ColliderComponent>())
            {
                auto collider          = m_owner->GetComponent<ColliderComponent>();
                m_collider_set         = collider->m_collider_set;
                collider->m_rigid_body = m_rigid_body;
                collider->Initialize();
                m_b_init = true;
            }
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_rigid_body->SetTransform(m_transform);
                m_rigid_body->SyncFromTransform(m_transform);
            }
        }
    }

    void RigidBodyComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void RigidBodyComponent::Shutdown()
    {
        Unsubscribe();
        if (m_rigid_body != nullptr)
        {
            m_rigid_body->Shutdown();
            delete m_rigid_body;
            m_rigid_body = nullptr;
        }
    }

    void RigidBodyComponent::ApplyForce(const Vector3& force, const Vector3& at) const
    {
        m_rigid_body->ApplyForce(force, at);
    }

    void RigidBodyComponent::ApplyForceCentroid(const Vector3& force) const
    {
        m_rigid_body->ApplyForceCentroid(force);
    }

    void RigidBodyComponent::SetLinearVelocity(const Vector3& linear) const
    {
        m_rigid_body->SetLinearVelocity(linear);
    }

    void RigidBodyComponent::SetAngularVelocity(const Vector3& angular) const
    {
        m_rigid_body->SetAngularVelocity(angular);
    }

    void RigidBodyComponent::SetMassInfinite() const
    {
        m_rigid_body->SetMassInfinite();
        m_rigid_body->SetInertiaInfinite();
    }

    void RigidBodyComponent::SetMass(Real mass) const
    {
        auto prev_inertia  = m_rigid_body->LocalInertia();
        auto prev_inv_mass = m_rigid_body->InverseMass();
        auto new_inertia   = (mass * prev_inv_mass) * prev_inertia;
        m_rigid_body->SetMass(mass);
        m_rigid_body->SetLocalInertia(new_inertia);
    }

    void RigidBodyComponent::SetPositionalConstraints(const Vector3& linear) const
    {
        m_rigid_body->SetPositionalConstraints(linear);
    }

    void RigidBodyComponent::SetRotationalConstraints(const Vector3& angular) const
    {
        m_rigid_body->SetRotationalConstraints(angular);
    }

    void RigidBodyComponent::SetMotionMode(eMotionMode motion_mode) const
    {
        m_rigid_body->SetMotionMode(motion_mode);
    }

    void RigidBodyComponent::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    Vector3 RigidBodyComponent::GetLinearVelocity() const
    {
        return m_rigid_body->GetLinearVelocity();
    }

    Vector3 RigidBodyComponent::GetAngularVelocity() const
    {
        return m_rigid_body->GetAngularVelocity();
    }

    Real RigidBodyComponent::GetMass() const
    {
        return m_rigid_body->Mass();
    }

    Real RigidBodyComponent::GetInverseMass() const
    {
        return m_rigid_body->InverseMass();
    }

    Matrix33 RigidBodyComponent::GetMassMatrix() const
    {
        return m_rigid_body->MassMatrix();
    }

    Matrix33 RigidBodyComponent::GetInverseMassMatrix() const
    {
        return m_rigid_body->InverseMassMatrix();
    }

    Matrix33 RigidBodyComponent::GetInertia() const
    {
        return m_rigid_body->Inertia();
    }

    Matrix33 RigidBodyComponent::GetInverseInertia() const
    {
        return m_rigid_body->InverseInertia();
    }

    eMotionMode RigidBodyComponent::GetMotionMode() const
    {
        return m_rigid_body->GetMotionMode();
    }

    RigidBody* RigidBodyComponent::GetRigidBody() const
    {
        return m_rigid_body;
    }

    bool RigidBodyComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Motion") && (data["Motion"].isString()))
        {
            std::string motion = data["Motion"].asString();
            if (motion == "Dynamic")
            {
                m_rigid_body->m_motion_mode = eMotionMode::Dynamic;
            }
            else if (motion == "Kinematic")
            {
                m_rigid_body->m_motion_mode = eMotionMode::Kinematic;
            }
            else if (motion == "Static")
            {
                m_rigid_body->m_motion_mode = eMotionMode::Static;
            }
            else
            {
                m_rigid_body->m_motion_mode = eMotionMode::Dynamic;
            }
        }
        //linear data
        if (JsonResource::HasMember(data, "Linear"))
        {
            auto linear = data["Linear"];
            if (JsonResource::HasMember(linear, "Position") && JsonResource::IsVector3(linear["Position"]))
            {
                m_rigid_body->m_transform->position = JsonResource::AsVector3(linear["Position"]);
            }
            if (JsonResource::HasMember(linear, "Velocity") && JsonResource::IsVector3(linear["Velocity"]))
            {
                m_rigid_body->m_linear_velocity = JsonResource::AsVector3(linear["Velocity"]);
            }
            if (JsonResource::HasMember(linear, "Force") && JsonResource::IsVector3(linear["Force"]))
            {
                m_rigid_body->m_force_accumulator = JsonResource::AsVector3(linear["Force"]);
            }
            if (JsonResource::HasMember(linear, "Constraints") && JsonResource::IsVector3(linear["Constraints"]))
            {
                m_rigid_body->m_constraints_positional = JsonResource::AsVector3(linear["Constraints"]);
            }
        }
        //angular data
        if (JsonResource::HasMember(data, "Angular"))
        {
            auto angular = data["Angular"];
            if (JsonResource::HasMember(angular, "Orientation") && JsonResource::IsQuaternion(angular["Orientation"]))
            {
                m_rigid_body->SetOrientation(JsonResource::AsQuaternionRIJK(angular["Orientation"]));
            }
            if (JsonResource::HasMember(angular, "Velocity") && JsonResource::IsVector3(angular["Velocity"]))
            {
                m_rigid_body->m_angular_velocity = JsonResource::AsVector3(angular["Velocity"]);
            }
            if (JsonResource::HasMember(angular, "Torque") && JsonResource::IsVector3(angular["Torque"]))
            {
                m_rigid_body->m_torque_accumulator = JsonResource::AsVector3(angular["Torque"]);
            }
            if (JsonResource::HasMember(angular, "Constraints") && JsonResource::IsVector3(angular["Constraints"]))
            {
                m_rigid_body->m_constraints_rotational = JsonResource::AsVector3(angular["Constraints"]);
            }
        }
        //mass data
        if (JsonResource::HasMember(data, "Mass"))
        {
            auto mass_data = data["Mass"];
            if (JsonResource::HasMember(mass_data, "Mass") && mass_data["Mass"].isDouble())
            {
                Real mass = mass_data["Mass"].asFloat();
                Math::IsZero(mass)
                    ? m_rigid_body->SetMassInfinite()
                    : m_rigid_body->SetMass(mass);
            }
            if (JsonResource::HasMember(mass_data, "Inertia") && JsonResource::IsMatrix33(mass_data["Inertia"]))
            {
                Matrix33 inertia_tensor = JsonResource::AsMatrix33(mass_data["Inertia"]);
                inertia_tensor.IsZero()
                    ? m_rigid_body->SetInertiaInfinite()
                    : m_rigid_body->SetLocalInertia(inertia_tensor);
            }
            if (JsonResource::HasMember(mass_data, "Centroid") && JsonResource::IsVector3(mass_data["Centroid"]))
            {
                m_rigid_body->m_mass_data.local_centroid = JsonResource::AsVector3(mass_data["Centroid"]);
                m_rigid_body->UpdateGlobalCentroidFromPosition();
            }
        }
        return true;
    }

    void RigidBodyComponent::Save(Json::Value& data) const
    {
    }

    void RigidBodyComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void RigidBodyComponent::Subscribe()
    {
        if (m_space != nullptr && m_rigid_body != nullptr)
        {
            m_space->GetWorld()->AddRigidBody(m_rigid_body);
        }
    }

    void RigidBodyComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_rigid_body != nullptr)
        {
            m_space->GetWorld()->RemoveRigidBody(m_rigid_body);
        }
    }

    RigidBodyComponent::RigidBodyComponent(Object* owner)
        : Component(owner)
    {
    }

    void RigidBodyComponent::Clone(RigidBodyComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
            Initialize();
            m_rigid_body->Clone(origin->m_rigid_body);
        }
    }
}
