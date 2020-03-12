#pragma once
#include "../Resource.hpp"

namespace Engine5
{
    class TextureResource : public Resource
    {
    public:
        explicit TextureResource(const std::wstring& path);
        ~TextureResource();

    private:
    };
}
