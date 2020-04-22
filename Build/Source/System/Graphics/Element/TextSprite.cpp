// ReSharper disable CppExpressionWithoutSideEffects
#include "TextSprite.hpp"
#include "../Buffer/TextBufferCommon.hpp"
#include "../../Math/Structure/Transform.hpp"

namespace Engine5
{
    TextSprite::TextSprite()
    {
    }

    TextSprite::~TextSprite()
    {
    }

    void TextSprite::Initialize(RendererCommon* renderer)
    {
        if (m_text == nullptr)
        {
            m_text = new TextBufferCommon();
            m_text->Initialize(renderer);
        }
    }

    void TextSprite::Render() const
    {
        m_text->Render();
    }

    void TextSprite::Shutdown()
    {
        if (m_text != nullptr)
        {
            m_text->Shutdown();
            delete m_text;
            m_text = nullptr;
        }
    }

    void TextSprite::SetText(const std::string& text) const
    {
        m_text->SetText(text);
        m_text->SetUpTextureSize();
        m_text->SetUpVertexBuffer();
        m_text->SetUpFontTexture();
        m_text->SetUpTextLayout();
    }

    void TextSprite::SetText(const std::wstring& text) const
    {
        m_text->SetText(text);
        m_text->SetUpTextureSize();
        m_text->SetUpVertexBuffer();
        m_text->SetUpFontTexture();
        m_text->SetUpTextLayout();
    }

    void TextSprite::SetFont(const std::wstring& font) const
    {
        m_text->SetFont(font);
        m_text->SetUpTextFormats();
        m_text->SetUpTextLayout();
    }

    void TextSprite::SetColor(const Color& color) const
    {
        m_text->SetColor(color);
    }

    Matrix44 TextSprite::GetModelMatrix() const
    {
        return m_transform->LocalToWorldMatrix();
    }

    TextureCommon* TextSprite::GetTexture() const
    {
        return m_text->GetTexture();
    }
}
