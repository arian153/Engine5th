#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeTextureAPI
#include <string>

namespace Engine5
{
    class RenderTextureBufferCommon;
    class TextTextureBufferCommon;
    class RendererCommon;

    class TextureCommon : public TextureAPI
    {
    public:
        TextureCommon();
        ~TextureCommon();

        bool Initialize(RendererCommon* renderer, const std::wstring& file_path, const std::wstring& file_type);
        bool Initialize(TextTextureBufferCommon* text_buffer);
        bool Initialize(RenderTextureBufferCommon* render_texture_buffer);
        bool Shutdown();
    private:
        bool m_b_buffer = false;
    };

   
}
