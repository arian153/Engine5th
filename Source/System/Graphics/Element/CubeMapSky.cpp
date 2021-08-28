#include "CubeMapSky.hpp"

#include "Camera.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "../Common/Texture/TextureCommon.hpp"
#include "../DataType/MeshData.hpp"
#include "../Utility/MeshGenerator.hpp"

namespace Engine5
{
    CubeMapSky::CubeMapSky()
    {
    }

    CubeMapSky::~CubeMapSky()
    {
    }

    void CubeMapSky::Initialize()
    {
        if (m_type == 0)
        {
            GenerateSkyBox();
        }
        else if (m_type == 1)
        {
            GenerateSkySphere();
        }

        //Build Buffer
        if (m_renderer != nullptr)
        {
            CreateBuffer();
        }
    }

    void CubeMapSky::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        m_transform.position = m_camera->GetPosition();
    }

    void CubeMapSky::Shutdown()
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

        m_indices.clear();
        m_vertices.clear();
    }

    void CubeMapSky::Bind() const
    {
        m_vertex_buffer->Bind(0);
        m_index_buffer->Bind(0);
        m_texture->Bind();
    }

    void CubeMapSky::Draw() const
    {
        m_index_buffer->Draw();
    }

    void CubeMapSky::GenerateSkySphere()
    {
        m_vertices.clear();
        m_indices.clear();
        MeshGenerator mesh_gen;
        MeshData      data;
        mesh_gen.CreateSphere(m_radius, 30, 30, data);

        size_t size = data.vertices.size();
        m_vertices.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            m_vertices[i] = data.vertices[i].GetPosition();
        }

        m_indices.assign(data.indices.begin(), data.indices.end());
    }

    void CubeMapSky::GenerateSkyBox()
    {
        m_vertices.clear();
        m_indices.clear();
        MeshGenerator mesh_gen;
        MeshData      data;
        mesh_gen.CreateBox(m_radius, m_radius, m_radius, 0, data);

        size_t size = data.vertices.size();
        m_vertices.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            m_vertices[i] = data.vertices[i].GetPosition();
        }

        m_indices.assign(data.indices.begin(), data.indices.end());
    }

    void CubeMapSky::SetCamera(Camera* camera)
    {
        m_camera = camera;
    }

    void CubeMapSky::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void CubeMapSky::SetTexture(TextureCommon* texture)
    {
        m_texture = texture;
    }

    void CubeMapSky::CreateBuffer()
    {
        if (m_index_buffer == nullptr)
        {
            m_index_buffer = new IndexBufferCommon();
        }

        if (m_vertex_buffer == nullptr)
        {
            m_vertex_buffer = new VertexBufferCommon();
        }

        m_index_buffer->Init(m_renderer, m_indices);
        m_vertex_buffer->Init(m_renderer, m_vertices, false);
    }

    Matrix44 CubeMapSky::GetModelMatrix() const
    {
        return m_transform.LocalToWorldMatrix();
    }
}
