#pragma once
#include <vector>

namespace Engine5
{
    class TextureCommon;

    class TextureArray
    {
    public:
        TextureArray();
        ~TextureArray();

    private:
        std::vector<TextureCommon*> m_textures;
    };
}
