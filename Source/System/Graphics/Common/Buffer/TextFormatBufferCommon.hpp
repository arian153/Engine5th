#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeTextFormatBufferAPI

namespace Engine5
{
    class RendererCommon;
    class Color;
    class Vector2;

    class TextFormatBufferCommon : public TextFormatBufferAPI
    {
    public:
        TextFormatBufferCommon();
        ~TextFormatBufferCommon();

        void Initialize(RendererCommon* renderer);
        void Shutdown();

    private:
    };
}
