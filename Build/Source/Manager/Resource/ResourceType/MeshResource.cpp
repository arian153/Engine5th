#include "MeshResource.hpp"

namespace Engine5
{
    MeshResource::MeshResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Mesh;
    }

    MeshResource::~MeshResource()
    {
    }

    void MeshResource::Initialize()
    {
    }

    void MeshResource::Shutdown()
    {
    }
}
