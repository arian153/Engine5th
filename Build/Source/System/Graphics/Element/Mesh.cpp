#include "Mesh.hpp"
#include "../DataType/MeshData.hpp"
#include "../Shader/ShaderManagerCommon.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../../Manager/Component/EngineComponent/MeshComponent.hpp"
#include "../Buffer/MeshBufferCommon.hpp"

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
        if (m_component != nullptr)
        {
            m_component->m_mesh = nullptr;
        }
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
            m_buffer->Render(m_stride, 0);
        }
    }

   
    void Mesh::BuildBuffer()
    {
        if (m_buffer == nullptr)
        {
            m_buffer = new MeshBufferCommon();
        }
        if (m_mesh_data != nullptr)
        {
            if (m_type == eShaderType::Invalid)
            {
                //skip build buffer
            }
            else if (m_type == eShaderType::Color)
            {
                std::vector<ColorVertexCommon> vertices;
                vertices.reserve(m_mesh_data->vertices.size());
                for (auto& vertex : m_mesh_data->vertices)
                {
                    vertices.emplace_back(vertex.GetPosition(), m_color);
                }
                m_buffer->BuildBuffer(m_renderer, vertices, m_mesh_data->indices);
                vertices.clear();
                m_stride = sizeof(ColorVertexCommon);
            }
            else
            {
                m_buffer->BuildBuffer(m_renderer, m_mesh_data->vertices, m_mesh_data->indices);
                m_stride = sizeof(NormalVertexCommon);
            }
        }
    }

    void Mesh::SetMeshData(MeshData* mesh_data)
    {
        m_mesh_data = mesh_data;
        BuildBuffer();
    }

    void Mesh::SetTexture(TextureCommon* texture)
    {
        m_texture = texture;
    }

    void Mesh::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void Mesh::SetShaderType(eShaderType type)
    {
        if (m_type != type)
        {
            m_type = type;
            BuildBuffer();
        }
    }

    void Mesh::SetColor(const Color& color)
    {
        m_color = color;
    }

    void Mesh::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
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

    eShaderType Mesh::GetShaderType() const
    {
        return m_type;
    }

    U32 Mesh::GetIndexCount() const
    {
        if (m_mesh_data != nullptr)
        {
            return static_cast<U32>(m_mesh_data->indices.size());
        }
        return 0;
    }

    Color Mesh::GetColor() const
    {
        return m_color;
    }

    bool Mesh::IsDeferred() const
    {
        return !(m_type == eShaderType::Color || m_type == eShaderType::Light || m_type == eShaderType::Invalid);
    }

    bool Mesh::IsForward() const
    {
        return m_type == eShaderType::Color || m_type == eShaderType::Light || m_type == eShaderType::Invalid;
    }
}
