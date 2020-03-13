#include "ShaderResource.hpp"

namespace Engine5
{
    ShaderResource::ShaderResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Shader;
    }

    ShaderResource::~ShaderResource()
    {
    }
}
