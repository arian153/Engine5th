#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeTextBufferAPI

namespace Engine5
{
    class RendererCommon;
    class Color;

    class TextBufferCommon : public TextBufferAPI
    {
    public:
        TextBufferCommon();
        ~TextBufferCommon();

        bool Initialize(RendererCommon* renderer);
        bool Update();
        void Render();
        void Shutdown();

        void SetText(const std::string& string) const;
        void SetText(const std::wstring& string) const;
        void SetFont(const std::wstring& font) const;
        void SetColor(Color color);

        U32   GetIndexCount() const;
        U32   GetVertexCount() const;
        Color GetColor() const;

    private:
    };
}
