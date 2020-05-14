#include "InstanceBufferDX11.hpp"
#include "../../../Buffer/InstanceBufferCommon.hpp"
#include "../../../Renderer/RendererCommon.hpp"

namespace Engine5
{
    InstanceBufferDX11::InstanceBufferDX11()
    {
    }

    InstanceBufferDX11::~InstanceBufferDX11()
    {
    }

    InstanceBufferCommon::InstanceBufferCommon()
    {
    }

    InstanceBufferCommon::~InstanceBufferCommon()
    {
    }

    void InstanceBufferCommon::Render(U32 stride, U32 offset) const
    {
        // Set the buffer strides.
        U32 strides[ 2 ];
        strides[0] = stride;
        strides[1] = sizeof(InstanceDataCommon);
        // Set the buffer offsets.
        U32 offsets[ 2 ];
        offsets[0] = offset;
        offsets[1] = 0;
        // Set the array of pointers to the vertex and instance buffers.
        ID3D11Buffer* buffers[ 2 ];
        buffers[0] = m_vertex_buffer;
        buffers[1] = m_instance_buffer;
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
        m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void InstanceBufferCommon::Shutdown()
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
        if (m_instance_buffer != nullptr)
        {
            m_instance_buffer->Release();
            m_instance_buffer = nullptr;
        }
    }

    bool InstanceBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, const std::vector<U32>& indices, const std::vector<InstanceDataCommon>& instances)
    {
        m_device         = renderer->GetDevice();
        m_device_context = renderer->GetDeviceContext();
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(ColorVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * static_cast<U32>(indices.size());;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices.data();
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = m_device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
        {
            return false;
        }
        if (instances.empty() == false)
        {
            // Set up the description of the instance buffer.
            D3D11_BUFFER_DESC instance_buffer_desc;
            instance_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
            instance_buffer_desc.ByteWidth           = sizeof(InstanceDataCommon) * static_cast<U32>(instances.size());
            instance_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            instance_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
            instance_buffer_desc.MiscFlags           = 0;
            instance_buffer_desc.StructureByteStride = 0;
            // Give the sub resource structure a pointer to the instance data.
            D3D11_SUBRESOURCE_DATA instance_data;
            instance_data.pSysMem          = instances.data();
            instance_data.SysMemPitch      = 0;
            instance_data.SysMemSlicePitch = 0;
            // Create the instance buffer.
            result = m_device->CreateBuffer(&instance_buffer_desc, &instance_data, &m_instance_buffer);
            if (FAILED(result))
            {
                return false;
            }
        }
        return true;
    }

    bool InstanceBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, const std::vector<U32>& indices, const std::vector<InstanceDataCommon>& instances)
    {
        m_device         = renderer->GetDevice();
        m_device_context = renderer->GetDeviceContext();
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(TextureVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * static_cast<U32>(indices.size());;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices.data();
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = m_device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
        {
            return false;
        }
        if (instances.empty() == false)
        {
            // Set up the description of the instance buffer.
            D3D11_BUFFER_DESC instance_buffer_desc;
            instance_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
            instance_buffer_desc.ByteWidth           = sizeof(InstanceDataCommon) * static_cast<U32>(instances.size());
            instance_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            instance_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
            instance_buffer_desc.MiscFlags           = 0;
            instance_buffer_desc.StructureByteStride = 0;
            // Give the sub resource structure a pointer to the instance data.
            D3D11_SUBRESOURCE_DATA instance_data;
            instance_data.pSysMem          = instances.data();
            instance_data.SysMemPitch      = 0;
            instance_data.SysMemSlicePitch = 0;
            // Create the instance buffer.
            result = m_device->CreateBuffer(&instance_buffer_desc, &instance_data, &m_instance_buffer);
            if (FAILED(result))
            {
                return false;
            }
        }
        return true;
    }

    bool InstanceBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, const std::vector<U32>& indices, const std::vector<InstanceDataCommon>& instances)
    {
        m_device         = renderer->GetDevice();
        m_device_context = renderer->GetDeviceContext();
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(NormalVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * static_cast<U32>(indices.size());;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices.data();
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = m_device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
        {
            return false;
        }
        if (instances.empty() == false)
        {
            // Set up the description of the instance buffer.
            D3D11_BUFFER_DESC instance_buffer_desc;
            instance_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
            instance_buffer_desc.ByteWidth           = sizeof(InstanceDataCommon) * static_cast<U32>(instances.size());
            instance_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            instance_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
            instance_buffer_desc.MiscFlags           = 0;
            instance_buffer_desc.StructureByteStride = 0;
            // Give the sub resource structure a pointer to the instance data.
            D3D11_SUBRESOURCE_DATA instance_data;
            instance_data.pSysMem          = instances.data();
            instance_data.SysMemPitch      = 0;
            instance_data.SysMemSlicePitch = 0;
            // Create the instance buffer.
            result = m_device->CreateBuffer(&instance_buffer_desc, &instance_data, &m_instance_buffer);
            if (FAILED(result))
            {
                return false;
            }
        }
        return true;
    }

    void InstanceBufferCommon::UpdateInstanceBuffer(const std::vector<InstanceDataCommon>& instances)
    {
        if (instances.empty() == false)
        {
            if (m_instance_buffer != nullptr)
            {
                m_instance_buffer->Release();
                m_instance_buffer = nullptr;
            }
            // Set up the description of the instance buffer.
            D3D11_BUFFER_DESC instance_buffer_desc;
            instance_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
            instance_buffer_desc.ByteWidth           = sizeof(InstanceDataCommon) * static_cast<U32>(instances.size());
            instance_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            instance_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
            instance_buffer_desc.MiscFlags           = 0;
            instance_buffer_desc.StructureByteStride = 0;
            // Give the sub resource structure a pointer to the instance data.
            D3D11_SUBRESOURCE_DATA instance_data;
            instance_data.pSysMem          = instances.data();
            instance_data.SysMemPitch      = 0;
            instance_data.SysMemSlicePitch = 0;
            // Create the instance buffer.
            HRESULT result = m_device->CreateBuffer(&instance_buffer_desc, &instance_data, &m_instance_buffer);
            if (FAILED(result))
            {
                return;
            }
            //if (m_instance_buffer == nullptr)
            //{
            //    // Set up the description of the instance buffer.
            //    D3D11_BUFFER_DESC instance_buffer_desc;
            //    instance_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
            //    instance_buffer_desc.ByteWidth           = sizeof(InstanceDataCommon) * static_cast<U32>(instances.size());
            //    instance_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            //    instance_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
            //    instance_buffer_desc.MiscFlags           = 0;
            //    instance_buffer_desc.StructureByteStride = 0;
            //    // Give the sub resource structure a pointer to the instance data.
            //    D3D11_SUBRESOURCE_DATA instance_data;
            //    instance_data.pSysMem          = instances.data();
            //    instance_data.SysMemPitch      = 0;
            //    instance_data.SysMemSlicePitch = 0;
            //    // Create the instance buffer.
            //    HRESULT result = m_device->CreateBuffer(&instance_buffer_desc, &instance_data, &m_instance_buffer);
            //    if (FAILED(result))
            //    {
            //        return;
            //    }
            //}
            //else
            //{
            //    // Lock the instance buffer so it can be written to.
            //    D3D11_MAPPED_SUBRESOURCE mapped_resource;
            //    // mapping
            //    HRESULT result = m_device_context->Map(m_instance_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            //    if (FAILED(result))
            //    {
            //        return;
            //    }
            //    // Get a pointer to the data in the instance buffer.
            //    InstanceDataCommon* instances_ptr = (InstanceDataCommon*)mapped_resource.pData;
            //    // Copy the data into the vertex buffer.
            //    memcpy(instances_ptr, (void*)instances.data(), sizeof(InstanceDataCommon) * static_cast<U32>(instances.size()));
            //    // Unlock the instance buffer.
            //    m_device_context->Unmap(m_instance_buffer, 0);
            //}
        }
    }
}
