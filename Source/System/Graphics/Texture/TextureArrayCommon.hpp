#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeTextureArrayAPI

namespace Engine5
{
    class TextureCommon;

    class TextureArrayCommon : public TextureArrayAPI
    {
    public:
        TextureArrayCommon();
        ~TextureArrayCommon();

        void PushFront(TextureCommon* texture);
        void PushBack(TextureCommon* texture);
        void Erase(TextureCommon* texture);
        void Clear();

        TextureCommon* Front() const;
        size_t         Size() const;

    private:
    };
}
