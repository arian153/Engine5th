#pragma once
#include <string>
#include "../../Math/Algebra/Matrix44.hpp"
#include "../Texture/TextureCommon.hpp"

namespace Engine5
{
    class Transform;
    class Color;
    class TextBufferCommon;

    class TextSprite
    {
    public:
        TextSprite();
        ~TextSprite();

        void Initialize(RendererCommon* renderer);
        void Render() const;
        void Shutdown();

        void SetText(const std::string& text) const;
        void SetText(const std::wstring& text) const;
        void SetFont(const std::wstring& font) const;
        void SetColor(const Color& color) const;

        Matrix44       GetModelMatrix() const;
        TextureCommon* GetTexture() const;

    private:
        TextBufferCommon* m_text      = nullptr;
        Transform*        m_transform = nullptr;
    };
}
