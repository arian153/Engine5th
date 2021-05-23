#include "Mesh2.hpp"

#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Structure/Transform.hpp"
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
        m_vertex_buffer->Bind(m_stride, 0, m_instance_buffer);
        m_index_buffer->Bind(0, m_instance_count);
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

        Transform transform;
        transform.position = Vector3(0, 0, 0);
        transform.scale    = Vector3(1, 1, 1);
        transform.orientation.Set(AxisRadian(Vector3(1, 1, 1).Unit(), Math::PI_DIV_6));

        m_instances.push_back({transform.LocalToWorldMatrix().Transpose(), Color()});

        transform.position = Vector3(3, 3, 3);
        transform.scale    = Vector3(1, 1, 1);
        transform.orientation.Set(AxisRadian(Vector3(1, 1, 1).Unit(), Math::PI_DIV_6 * 2.0f));

        m_instances.push_back({transform.LocalToWorldMatrix().Transpose(), Color(0, 0, 0, 1)});

        transform.position = Vector3(-3, 3, 3);
        transform.scale    = Vector3(1, 1, 1);
        transform.orientation.Set(AxisRadian(Vector3(1, 1, 1).Unit(), Math::PI_DIV_6 * 3.0f));

        m_instances.push_back({transform.LocalToWorldMatrix().Transpose(), Color(1, 1, 0, 1)});

        transform.position = Vector3(3, -3, 3);
        transform.scale    = Vector3(1, 1, 1);
        transform.orientation.Set(AxisRadian(Vector3(1, 1, 1).Unit(), Math::PI_DIV_6 * 4.0f));

        m_instances.push_back({transform.LocalToWorldMatrix().Transpose(), Color(0.5, 0.5, 0.5, 1)});

        transform.position = Vector3(3, 3, -3);
        transform.scale    = Vector3(1, 1, 1);
        transform.orientation.Set(AxisRadian(Vector3(1, 1, 1).Unit(), Math::PI_DIV_6 * 5.0f));

        m_instances.push_back({transform.LocalToWorldMatrix().Transpose(), Color(0.3, 0.3, 0.3, 1)});

        transform.position = Vector3(-3, -3, 3);
        transform.scale    = Vector3(1, 1, 1);
        transform.orientation.Set(AxisRadian(Vector3(1, 1, 1).Unit(), Math::PI_DIV_6 * 6.0f));

        m_instances.push_back({transform.LocalToWorldMatrix().Transpose(), Color(0.7, 0.7, 0.7, 1)});

        m_instance_buffer->Init(m_renderer, m_instances);
        m_instance_count = (U32)m_instances.size();
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
