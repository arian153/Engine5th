#include "RenderTextureBufferDX11.hpp"
#include "../../../Buffer/RenderTextureBufferCommon.hpp"
#include "../../../Vertex/TextureVertexCommon.hpp"

namespace Engine5
{
    RenderTextureBufferDX11::RenderTextureBufferDX11()
    {
    }

    RenderTextureBufferDX11::~RenderTextureBufferDX11()
    {
    }

    void RenderTextureBufferDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void RenderTextureBufferDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    RenderTextureBufferCommon::RenderTextureBufferCommon()
    {
    }

    RenderTextureBufferCommon::~RenderTextureBufferCommon()
    {
    }

    bool RenderTextureBufferCommon::Initialize(Real width, Real height)
    {
        Real left      = -width * 0.5f;
        Real right     = left + width;
        Real top       = height * 0.5f;
        Real bottom    = top - height;
        m_vertex_count = 4;
        m_index_count  = 6;
        // Create the vertex array.
        TextureVertexCommon* vertices = new TextureVertexCommon[ m_vertex_count ];
        if (vertices == nullptr)
        {
            return false;
        }
        // Create the index array.
        U32* indices = new U32[ m_index_count ];
        if (indices == nullptr)
        {
            return false;
        }
        // Load the vertex array with data.
        // First triangle.
        vertices[0] = TextureVertexCommon(left, top, 0.0f, 0.0f, 0.0f);
        vertices[1] = TextureVertexCommon(right, bottom, 0.0f, 1.0f, 1.0f);
        vertices[2] = TextureVertexCommon(left, bottom, 0.0f, 0.0f, 1.0f);
        vertices[3] = TextureVertexCommon(right, top, 0.0f, 1.0f, 0.0f);
        // Load the index array with data.
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 3;
        indices[5] = 1;
        // Set up the description of the vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(TextureVertexCommon) * m_vertex_count;
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices;
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now finally create the vertex buffer.
        HRESULT result = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Set up the description of the index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(unsigned long) * m_index_count;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices;
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = m_device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
        {
            return false;
        }
        delete[] vertices;
        vertices = nullptr;
        delete[] indices;
        indices = nullptr;
        return true;
    }

    void RenderTextureBufferCommon::Render() const
    {
        // Set vertex buffer stride and offset.
        U32 stride = sizeof(TextureVertexCommon);
        U32 offset = 0;
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
        m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void RenderTextureBufferCommon::Shutdown()
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Release();
            m_index_buffer = nullptr;
        }
    }

    bool RenderTextureBufferCommon::OnResize(Real width, Real height)
    {
        Shutdown();
        return Initialize(width, height);
    }

    U32 RenderTextureBufferCommon::GetIndexCount() const
    {
        return m_index_count;
    }
}
