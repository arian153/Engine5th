#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeTextLayoutBufferAPI

namespace Engine5
{
    class RendererCommon;
    class Color;
    class Vector2;

    class TextLayoutBufferCommon : public TextLayoutBufferAPI
    {
    public:
        TextLayoutBufferCommon();
        ~TextLayoutBufferCommon();

        void Initialize(RendererCommon* renderer);
        void Render() const;
        void Shutdown();

        bool SetPosition(const Vector2& position);
        bool SetColor(const Color& color);

    private:
    };
}
