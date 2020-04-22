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

        void Initialize();
        void Render();
        void Shutdown();

        void SetText(const std::string& text);
        void SetText(const std::wstring& text);
        void SetFont(const std::wstring& font);
        void SetColor(const Color& color);

        Matrix44       GetModelMatrix() const;
        TextureCommon* GetTexture() const;

    private:
        TextBufferCommon* m_text      = nullptr;
        Transform*        m_transform = nullptr;
    };
}
