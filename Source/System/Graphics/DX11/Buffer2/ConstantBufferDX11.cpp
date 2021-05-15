#include "ConstantBufferDX11.hpp"

#include "../../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    ConstantBufferDX11::ConstantBufferDX11()
    {
    }

    ConstantBufferDX11::~ConstantBufferDX11()
    {
    }

    ConstantBufferCommon::ConstantBufferCommon()
    {
    }

    ConstantBufferCommon::~ConstantBufferCommon()
    {
    }

    bool ConstantBufferCommon::Init(RendererCommon* renderer, eBindingStage type, size_t buffer_size)
    {
        if (m_constant_buffer != nullptr)
        {
            m_constant_buffer->Release();
            m_constant_buffer = nullptr;
        }

        m_binding_type = type;
        U32 byte_width = (buffer_size + 255) & ~255;

        D3D11_BUFFER_DESC buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth           = byte_width;
        buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags           = 0;
        buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_constant_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void ConstantBufferCommon::Update(void* data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        mapped_resource.pData = data;
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Bind(U32 slot) const
    {
        if (m_binding_type == eBindingStage::VertexShader)
            m_device_context->VSSetConstantBuffers(slot, 1, &m_constant_buffer);

        if (m_binding_type == eBindingStage::PixelShader)
            m_device_context->PSSetConstantBuffers(slot, 1, &m_constant_buffer);
    }

    void ConstantBufferCommon::Shutdown()
    {
        if (m_constant_buffer != nullptr)
        {
            m_constant_buffer->Release();
            m_constant_buffer = nullptr;
        }
    }
}
