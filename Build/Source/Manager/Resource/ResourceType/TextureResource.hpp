#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class TextureResource final : public Resource
    {
    public:
        explicit TextureResource(const std::wstring& path);
        ~TextureResource();

    private:
    };
}
