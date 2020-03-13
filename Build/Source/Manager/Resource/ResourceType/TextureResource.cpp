#include "TextureResource.hpp"

namespace Engine5
{
    TextureResource::TextureResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Texture;
    }

    TextureResource::~TextureResource()
    {
    }
}
