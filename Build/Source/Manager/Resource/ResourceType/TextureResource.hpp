#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class TextureCommon;
    class TextureResource final : public Resource
    {
    public:
        explicit TextureResource(const std::wstring& path);
        ~TextureResource();

        TextureCommon* GetTexture() const;

    private:
        TextureCommon* m_texture = nullptr;
    };
}
