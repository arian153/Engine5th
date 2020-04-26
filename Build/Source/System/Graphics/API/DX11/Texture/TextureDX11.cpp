#include "TextureDX11.hpp"
#include "../../../Texture/TextureCommon.hpp"
#include "../../../../../External/DirectXTextureLoader/DDSTextureLoader.h"
#include "../../../../../External/DirectXTextureLoader/WICTextureLoader.h"
#include "../../../Renderer/RendererCommon.hpp"
#include "../../../Buffer/TextBufferCommon.hpp"
#include "../../../Buffer/RenderTextureBufferCommon.hpp"

namespace Engine5
{
    TextureDX11::TextureDX11()
    {
    }

    TextureDX11::~TextureDX11()
    {
    }

    ID3D11ShaderResourceView* TextureDX11::GetTexture() const
    {
        return m_texture;
    }

    TextureCommon::TextureCommon()
    {
    }

    TextureCommon::~TextureCommon()
    {
    }

    bool TextureCommon::Initialize(RendererCommon* renderer, const std::wstring& file_path, const std::wstring& file_type)
    {
        HRESULT result;
        if (file_type == L".dds" || file_type == L".DDS")
        {
            result = DirectX::CreateDDSTextureFromFile
                    (
                     renderer->GetDevice(),
                     renderer->GetDeviceContext(),
                     file_path.c_str(),
                     &m_texture_resource,
                     &m_texture
                    );
        }
        else if (
            file_type == L".png" || file_type == L".PNG" ||
            file_type == L".jpeg" || file_type == L".JPEG" ||
            file_type == L".jpg" || file_type == L".JPG" ||
            file_type == L".bmp" || file_type == L".BMP" ||
            file_type == L".tiff" || file_type == L".TIFF" ||
            file_type == L".gif" || file_type == L".GIF")
        {
            result = DirectX::CreateWICTextureFromFile
                    (
                     renderer->GetDevice(),
                     renderer->GetDeviceContext(),
                     file_path.c_str(),
                     &m_texture_resource,
                     &m_texture
                    );
        }
        else
        {
            return false;
        }
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    bool TextureCommon::Initialize(TextBufferCommon* text_buffer)
    {
        m_texture  = text_buffer->GetTextResource();
        m_b_buffer = true;
        return true;
    }

    bool TextureCommon::Initialize(RenderTextureBufferCommon* render_texture_buffer)
    {
        m_texture  = render_texture_buffer->GetTextureResource();
        m_b_buffer = true;
        return true;
    }

    bool TextureCommon::Shutdown()
    {
        if (m_b_buffer == false)
        {
            if (m_texture != nullptr)
            {
                m_texture->Release();
                m_texture = nullptr;
            }
            if (m_texture_resource != nullptr)
            {
                m_texture_resource->Release();
                m_texture_resource = nullptr;
            }
        }
        return true;
    }
}
