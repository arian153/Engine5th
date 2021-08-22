#include "TextureArrayDX11.hpp"
#include "../../Common/Texture/TextureArrayCommon.hpp"
#include "../../Common/Texture/TextureCommon.hpp"

namespace Engine5
{
    TextureArrayDX11::TextureArrayDX11()
    {
    }

    TextureArrayDX11::~TextureArrayDX11()
    {
    }

    ID3D11ShaderResourceView** TextureArrayDX11::Data()
    {
        return m_textures.data();
    }

    ID3D11ShaderResourceView* const* TextureArrayDX11::Data() const
    {
        return m_textures.data();
    }

    TextureArrayCommon::TextureArrayCommon()
    {
    }

    TextureArrayCommon::~TextureArrayCommon()
    {
    }

    void TextureArrayCommon::PushFront(TextureCommon* texture)
    {
        m_front = texture;
        m_textures.insert(m_textures.begin(), texture->GetTexture());
    }

    void TextureArrayCommon::PushBack(TextureCommon* texture)
    {
        if (m_front == nullptr || m_textures.empty())
        {
            m_front = texture;
        }
        m_textures.push_back(texture->GetTexture());
    }

    void TextureArrayCommon::Erase(TextureCommon* texture)
    {
        auto found = std::find(m_textures.begin(), m_textures.end(), texture->GetTexture());
        m_textures.erase(found);
        if (m_textures.empty())
        {
            m_front = nullptr;
        }
    }

    void TextureArrayCommon::Clear()
    {
        m_textures.clear();
        m_front = nullptr;
    }

    TextureCommon* TextureArrayCommon::Front() const
    {
        return m_front;
    }

    size_t TextureArrayCommon::Size() const
    {
        return m_textures.size();
    }
}
