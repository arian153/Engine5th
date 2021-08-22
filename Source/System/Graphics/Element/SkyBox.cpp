#include "SkyBox.hpp"

#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "../Common/Texture/TextureCommon.hpp"

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
        auto texture = m_texture->GetTexture();
        m_renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
    }

    void SkyBox::Draw() const
    {
        m_index_buffer->Draw();
    }
}
