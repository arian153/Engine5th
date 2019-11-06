#include "Mesh.hpp"

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
        

        // Set the number of vertices in the vertex array.
        m_vertex_count = 3;

        // Set the number of indices in the index array.
        m_index_count = 3;

        // Create the vertex array.
        VertexType* vertices = new VertexType[m_vertex_count];
        if (!vertices)
        {
            return;
        }

        // Create the index array.
        unsigned long* indices = new unsigned long[m_index_count];
        if (!indices)
        {
            return;
        }

        // Load the vertex array with data.
        vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
        vertices[0].color    = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
        vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
        vertices[1].color    = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
        vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
        vertices[2].color    = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        // Load the index array with data.
        indices[0] = 0;  // Bottom left.
        indices[1] = 1;  // Top middle.
        indices[2] = 2;  // Bottom right.
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(VertexType) * m_vertex_count;
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
         
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices;
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;

        // Now create the vertex buffer.
        HRESULT result = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return;
        }

        // Set up the description of the static index buffer.
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
            return;
        }

        // Release the arrays now that the vertex and index buffers have been created and loaded.
        delete[] vertices;
        vertices = nullptr;
        delete[] indices;
        indices = nullptr;
    }

    void Mesh::Update(Real dt)
    {
    }

    void Mesh::Shutdown()
    {
        // Release the index buffer.
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Release();
            m_index_buffer = nullptr;
        }

        // Release the vertex buffer.
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
    }

    void Mesh::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
    {
        m_device         = device;
        m_device_context = context;
    }

    int Mesh::GetIndexCount() const
    {
        return m_index_count;
    }

    void Mesh::RenderBuffers() const
    {
        // Set vertex buffer stride and offset.
        unsigned int stride = sizeof(VertexType);
        unsigned int offset = 0;

        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

        // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
        m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}
