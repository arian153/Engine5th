#include "ParticleEmitterComponent.hpp"
#include "../../../System/Graphics/Element/ParticleEmitter.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/TextureResource.hpp"

namespace Engine5
{
    ParticleEmitterComponent::~ParticleEmitterComponent()
    {
    }

    void ParticleEmitterComponent::Initialize()
    {
        if (m_emitter == nullptr)
        {
            m_emitter = new ParticleEmitter();
            m_space->GetScene()->InitializeParticleEmitter(m_emitter);
            m_emitter->m_component = this;
            Subscribe();
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_emitter->SetTransform(m_transform);
            }
        }
    }

    void ParticleEmitterComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void ParticleEmitterComponent::Shutdown()
    {
        Unsubscribe();
        if (m_emitter != nullptr)
        {
            m_emitter->Shutdown();
            delete m_emitter;
            m_emitter = nullptr;
        }
    }

    bool ParticleEmitterComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Texture") && data["Texture"].isString())
        {
            std::string texture  = data["Texture"].asString();
            auto        resource = m_space->GetResourceManager()->GetTextureResource(ToWString(texture));
            m_emitter->SetTexture(resource->GetTexture());
        }
        if (JsonResource::HasMember(data, "Particle Amount") && data["Particle Amount"].isInt())
        {
            size_t amount = data["Particle Amount"].asLargestUInt();
            m_emitter->SetParticleAmount(amount);
        }
        if (JsonResource::HasMember(data, "Emission Amount") && data["Emission Amount"].isInt())
        {
            size_t amount = data["Emission Amount"].asLargestUInt();
            m_emitter->SetEmissionAmount(amount);
        }
        if (JsonResource::HasMember(data, "Emission Rate") && data["Emission Rate"].isDouble())
        {
            Real rate = data["Emission Rate"].asFloat();
            m_emitter->SetEmissionRate(rate);
        }
        if (JsonResource::HasMember(data, "Life Decay Rate") && data["Life Decay Rate"].isDouble())
        {
            Real rate = data["Life Decay Rate"].asFloat();
            m_emitter->SetLifeDecayRate(rate);
        }
        if (JsonResource::HasMember(data, "Scale Decay Rate") && data["Scale Decay Rate"].isDouble())
        {
            Real rate = data["Scale Decay Rate"].asFloat();
            m_emitter->SetScaleDecayRate(rate);
        }
        if (JsonResource::HasMember(data, "Base Position") && JsonResource::IsVector3(data["Base Position"]))
        {
            auto value = JsonResource::AsVector3(data["Base Position"]);
            m_emitter->SetBasePosition(value);
        }
        if (JsonResource::HasMember(data, "Base Velocity") && JsonResource::IsVector3(data["Base Velocity"]))
        {
            auto value = JsonResource::AsVector3(data["Base Velocity"]);
            m_emitter->SetBaseVelocity(value);
        }
        if (JsonResource::HasMember(data, "Base Color") && JsonResource::IsColor(data["Base Color"]))
        {
            auto value = JsonResource::AsColor(data["Base Color"]);
            m_emitter->SetBaseColor(value);
        }
        if (JsonResource::HasMember(data, "Base Life") && data["Base Life"].isDouble())
        {
            Real value = data["Base Life"].asFloat();
            m_emitter->SetBaseLife(value);
        }
        if (JsonResource::HasMember(data, "Base Scale") && data["Base Scale"].isDouble())
        {
            Real value = data["Base Scale"].asFloat();
            m_emitter->SetBaseScale(value);
        }
        if (JsonResource::HasMember(data, "Position Variance") && JsonResource::IsVector3(data["Position Variance"]))
        {
            auto variance = JsonResource::AsVector3(data["Position Variance"]);
            m_emitter->SetPositionVariance(variance);
        }
        if (JsonResource::HasMember(data, "Direction Variance") && JsonResource::IsVector3(data["Direction Variance"]))
        {
            auto variance = JsonResource::AsVector3(data["Direction Variance"]);
            m_emitter->SetDirectionVariance(variance);
        }
        if (JsonResource::HasMember(data, "Color Variance") && JsonResource::IsColor(data["Color Variance"]))
        {
            auto variance = JsonResource::AsColor(data["Color Variance"]);
            m_emitter->SetColorVariance(variance);
        }
        if (JsonResource::HasMember(data, "Speed Variance") && data["Speed Variance"].isDouble())
        {
            Real variance = data["Speed Variance"].asFloat();
            m_emitter->SetSpeedVariance(variance);
        }
        if (JsonResource::HasMember(data, "Life Variance") && data["Life Variance"].isDouble())
        {
            Real variance = data["Life Variance"].asFloat();
            m_emitter->SetLifeVariance(variance);
        }
        if (JsonResource::HasMember(data, "Scale Variance") && data["Scale Variance"].isDouble())
        {
            Real variance = data["Scale Variance"].asFloat();
            m_emitter->SetScaleVariance(variance);
        }
        return true;
    }

    void ParticleEmitterComponent::Save(Json::Value& data) const
    {
    }

    void ParticleEmitterComponent::Edit()
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void ParticleEmitterComponent::Subscribe()
    {
        if (m_space != nullptr && m_emitter != nullptr)
        {
            m_space->GetScene()->AddParticleEmitter(m_emitter);
        }
    }

    void ParticleEmitterComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_emitter != nullptr)
        {
            m_space->GetScene()->RemoveParticleEmitter(m_emitter);
        }
    }

    ParticleEmitterComponent::ParticleEmitterComponent(Object* owner)
        : Component(owner)
    {
    }

    void ParticleEmitterComponent::Clone(ParticleEmitterComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
        }
    }
}
