#include "MeshResource.hpp"
#include <fstream>

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

    void MeshResource::Load()
    {
        std::ifstream file_stream;
        // Open the model file.
        file_stream.open(m_file_path);
        // If it could not open the file then exit.
        if (file_stream.fail())
        {
            return;
        }
        m_mesh_data.b_resource = true;
        CheckMeshType();
        switch (m_mesh_type)
        {
        case eMeshType::Invalid:
            break;
        case eMeshType::CustomTXT:
            LoadCustomTXT();
            break;
        case eMeshType::WaveFrontOBJ:
            LoadWaveFrontOBJ();
            break;
        default:
            break;
        }
        file_stream.close();
    }

    void MeshResource::LoadWaveFrontOBJ()
    {
    }

    void MeshResource::LoadCustomTXT()
    {
    }

    void MeshResource::CheckMeshType()
    {
    }
}
