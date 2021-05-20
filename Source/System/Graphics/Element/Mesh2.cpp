#include "Mesh2.hpp"

#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/DataType/MeshData.hpp"

namespace Engine5
{
    Mesh2::Mesh2()
    {
    }

    Mesh2::~Mesh2()
    {
    }

    void Mesh2::Initialize()
    {
    }

    void Mesh2::Shutdown()
    {
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Shutdown();
            delete m_index_buffer;
            m_index_buffer = nullptr;
        }

        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Shutdown();
            delete m_vertex_buffer;
            m_vertex_buffer = nullptr;
        }

        if (m_mesh_data != nullptr && m_mesh_data->b_resource == false)
        {
            m_mesh_data->indices.clear();
            m_mesh_data->vertices.clear();
            delete m_mesh_data;
            m_mesh_data = nullptr;
        }
        m_texture_array.Clear();
    }

    void Mesh2::RenderBuffer() const
    {
        m_vertex_buffer->Bind(m_stride, 0);
        m_index_buffer->Bind(0);
    }

    void Mesh2::BuildBuffer()
    {
    }

    void Mesh2::SetModelData(MeshData* data)
    {
    }

    void Mesh2::AddTexture(TextureCommon* texture)
    {
    }

    void Mesh2::ClearTexture()
    {
    }

    void Mesh2::RemoveTexture(TextureCommon* texture)
    {
    }

    void Mesh2::SetTransform(Transform* transform)
    {
    }

    void Mesh2::SetRenderer(RendererCommon* renderer)
    {
    }
}
