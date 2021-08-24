#include "SkyBox.hpp"


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
    SkyBox::SkyBox()
    {
    }

    SkyBox::~SkyBox()
    {
    }

    void SkyBox::Initialize()
    {
        //Add Cube Vertices

        //Add Cube Indices

        //Build Buffer
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

    void SkyBox::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        m_transform.position = m_camera->GetPosition();
    }

    void SkyBox::Shutdown()
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

    void SkyBox::Bind() const
    {
        m_vertex_buffer->Bind(0);
        m_index_buffer->Bind(0);
        m_texture->Bind();
    }

    void SkyBox::Draw() const
    {
        m_index_buffer->Draw();
    }

    void SkyBox::GenerateSkySphere()
    {
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

    void SkyBox::SetCamera(Camera* camera)
    {
        m_camera = camera;
    }

    Matrix44 SkyBox::GetModelMatrix() const
    {
        return m_transform.LocalToWorldMatrix();
    }
}
