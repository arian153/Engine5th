#pragma once
#include <d3d11.h>

namespace Engine5
{
    class TextureDX11
    {
    public:
        TextureDX11();
        ~TextureDX11();

        ID3D11ShaderResourceView* GetTexture() const;
    protected:
        ID3D11ShaderResourceView* m_texture          = nullptr;
        ID3D11Resource*           m_texture_resource = nullptr;
    };
}
