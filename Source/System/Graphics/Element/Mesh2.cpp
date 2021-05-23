#include "Mesh2.hpp"

#include "../../Math/Algebra/Vector3.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/InstanceBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/DataType/Color.hpp"
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

        if (m_instance_buffer != nullptr)
        {
            m_instance_buffer->Shutdown();
            delete m_instance_buffer;
            m_instance_buffer = nullptr;
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

    void Mesh2::Render() const
    {
        m_vertex_buffer->Bind(m_stride, 0);
        m_index_buffer->Bind(0);
    }

    void Mesh2::BuildBuffer()
    {
        if (m_index_buffer == nullptr)
        {
            m_index_buffer = new IndexBufferCommon();
        }

        if (m_vertex_buffer == nullptr)
        {
            m_vertex_buffer = new VertexBufferCommon();
        }

        if (m_instance_buffer == nullptr)
        {
            m_instance_buffer = new InstanceBufferCommon();
        }

        //temp code
        std::vector<ColorVertexCommon> vertices(3);

        vertices[0] = ColorVertexCommon(Vector3(-1.0f, -1.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f));
        vertices[1] = ColorVertexCommon(Vector3(0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 0.0f, 1.0f));
        vertices[2] = ColorVertexCommon(Vector3(1.0f, -1.0f, 0.0f), Color(0.0f, 0.0f, 1.0f, 1.0f));

        std::vector<U32> indices = {0, 1, 2};
        m_index_buffer->Init(m_renderer, indices);
        m_vertex_buffer->Init(m_renderer, vertices, false);
        m_stride = sizeof(ColorVertexCommon);
    }

    void Mesh2::AddInstance(const InstanceBufferData& data)
    {
        if (m_instances.size() == m_max_count)
        {
            m_max_count = (m_max_count + 1) * 2;
            //then resize instance buffer.
            ResizeInstanceBuffer(m_max_count);
        }

        m_instances.push_back(data);
    }

    void Mesh2::SetModelData(MeshData* data)
    {
        m_mesh_data = data;
        BuildBuffer();
    }

    void Mesh2::AddTexture(TextureCommon* texture)
    {
        m_texture_array.PushBack(texture);
    }

    void Mesh2::ClearTexture()
    {
        m_texture_array.Clear();
    }

    void Mesh2::RemoveTexture(TextureCommon* texture)
    {
        m_texture_array.Erase(texture);
    }

    void Mesh2::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void Mesh2::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void Mesh2::ResizeInstanceBuffer(U32 count)
    {
        m_max_count = count;
        m_instances.resize(count);
        m_instance_buffer->Init(m_renderer, m_instances);
    }
}
