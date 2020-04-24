#include "TextSpriteComponent.hpp"
#include "../../../System/Graphics/Element/TextSprite.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Resource/ResourceType/JsonResource.hpp"

namespace Engine5
{
    TextSpriteComponent::~TextSpriteComponent()
    {
    }

    void TextSpriteComponent::Initialize()
    {
        if (m_text_sprite == nullptr)
        {
            m_text_sprite = new TextSprite();
            m_space->GetScene()->InitializeTextSprite(m_text_sprite);
            m_text_sprite->m_component = this;
            Subscribe();
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_text_sprite->SetTransform(m_transform);
            }
        }
    }

    void TextSpriteComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void TextSpriteComponent::Shutdown()
    {
        Unsubscribe();
        if (m_text_sprite != nullptr)
        {
            m_text_sprite->Shutdown();
            delete m_text_sprite;
            m_text_sprite = nullptr;
        }
    }

  
    bool TextSpriteComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Color") && JsonResource::IsColor(data[ "Color" ]))
        {
            m_text_sprite->SetColor(JsonResource::AsColor(data[ "Color" ]));
        }

        if (JsonResource::HasMember(data, "Font") && data[ "Font" ].isString())
        {
            m_text_sprite->SetFont(StringToWString(data[ "Font" ].asString()));
        }

        if (JsonResource::HasMember(data, "Text") && data[ "Text" ].isString())
        {
            m_text_sprite->SetText(StringToWString(data[ "Text" ].asString()));
        }

        return true;
    }

    void TextSpriteComponent::Save(Json::Value& data) const
    {
    }

    void TextSpriteComponent::Subscribe()
    {
        if (m_space != nullptr && m_text_sprite != nullptr)
        {
            m_space->GetScene()->AddTextSprite(m_text_sprite);
        }
    }

    void TextSpriteComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_text_sprite != nullptr)
        {
            m_space->GetScene()->RemoveTextSprite(m_text_sprite);
        }
    }

    TextSpriteComponent::TextSpriteComponent(Object* owner)
        : Component(owner)
    {
    }

    void TextSpriteComponent::Clone(TextSpriteComponent* origin)
    {
    }
}
