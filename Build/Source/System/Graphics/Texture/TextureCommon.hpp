#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeTextureAPI
#include <string>

namespace Engine5
{
    class RendererCommon;

    class TextureCommon : public TextureAPI
    {
    public:
        TextureCommon();
        ~TextureCommon();


        bool Initialize(RendererCommon* renderer, const std::wstring& file_path, const std::wstring& file_type);
        bool Shutdown();
    private:
    };

   
}
