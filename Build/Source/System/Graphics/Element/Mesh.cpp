#include "Mesh.hpp"
#include "../DataType/BufferCommon.hpp"
#include "../DataType/MeshData.hpp"

namespace Engine5
{
    Mesh::Mesh()
    {
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::Initialize()
    {
    }

    void Mesh::Render() const
    {
        if (m_buffer != nullptr)
        {
            m_buffer->Render(sizeof(TextureVertex), 0);
        }
    }

    void Mesh::Shutdown()
    {
        if (m_buffer != nullptr)
        {
            m_buffer->Shutdown();
            delete m_buffer;
            m_buffer = nullptr;
        }
        if (m_mesh_data != nullptr && m_mesh_data->b_resource == false)
        {
            m_mesh_data->indices.clear();
            m_mesh_data->vertices.clear();
            delete m_mesh_data;
            m_mesh_data = nullptr;
        }
    }

    void Mesh::Build()
    {
        if (m_buffer == nullptr)
        {
            m_buffer = new BufferCommon();
        }
        m_buffer->BuildBuffer(m_renderer, m_mesh_data->vertices, m_mesh_data->indices);
    }

    void Mesh::SetMeshData(MeshData* mesh_data)
    {
        m_mesh_data = mesh_data;
    }
}
