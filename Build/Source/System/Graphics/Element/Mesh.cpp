#include "Mesh.hpp"
#include "../DataType/BufferCommon.hpp"
#include "../DataType/MeshData.hpp"
#include "../Shader/ShaderManager.hpp"
#include "../../Math/Structure/Transform.hpp"

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

    void Mesh::RenderBuffer() const
    {
        if (m_buffer != nullptr)
        {
            m_buffer->Render(sizeof(TextureVertex), 0);
        }
    }

    void Mesh::BuildBuffer()
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

    void Mesh::SetTexture(TextureCommon* texture)
    {
        m_texture = texture;
    }

    void Mesh::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    Matrix44 Mesh::GetModelMatrix() const
    {
        if (m_transform != nullptr)
        {
            return m_transform->LocalToWorldMatrix();
        }
        return Matrix44();
    }

    TextureCommon* Mesh::GetTexture() const
    {
        return m_texture;
    }
}
