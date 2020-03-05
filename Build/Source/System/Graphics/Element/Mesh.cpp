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
        if (m_buffer == nullptr)
        {
            m_buffer = new BufferCommon();
        }
        m_buffer->BuildBuffer(m_renderer, m_mesh_data->vertices, m_mesh_data->indices);
    }

    void Mesh::Render() const
    {
        m_buffer->Render(sizeof(TextureVertex), 0);
    }

    void Mesh::Shutdown()
    {
        if (m_buffer != nullptr)
        {
            m_buffer->Shutdown();
            delete m_buffer;
            m_buffer = nullptr;
        }
    }
}
