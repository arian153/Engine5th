#pragma once
#include <vector>
#include <d3d11.h>

namespace Engine5
{
    class TextureCommon;

    class TextureArrayDX11
    {
    public:
        TextureArrayDX11();
        ~TextureArrayDX11();

        ID3D11ShaderResourceView**       Data();
        ID3D11ShaderResourceView* const* Data() const;

    protected:
        std::vector<ID3D11ShaderResourceView*> m_textures;
        TextureCommon*                         m_front = nullptr;
    };
}
