#include "ColorBufferDX11.hpp"

#include "../ConverterDX11.hpp"
#include "../../Common/Buffer2/ColorBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    ColorBufferDX11::ColorBufferDX11()
    {
    }

    ColorBufferDX11::~ColorBufferDX11()
    {
    }

    ColorBufferCommon::ColorBufferCommon()
    {
    }

    ColorBufferCommon::~ColorBufferCommon()
    {
    }

    bool ColorBufferCommon::Init(RendererCommon* renderer, eBindingStage type)
    {
        if (m_color_buffer != nullptr)
        {
            m_color_buffer->Release();
            m_color_buffer = nullptr;
        }

        m_binding_type = type;

        D3D11_BUFFER_DESC buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth           = sizeof(ColorBufferType);
        buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags           = 0;
        buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_color_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void ColorBufferCommon::Update(const Color& color) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_color_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;
        ColorBufferType* data_ptr = (ColorBufferType*)mapped_resource.pData;
        data_ptr->color           = ConverterDX11::ToXMFloat4(color);
        m_device_context->Unmap(m_color_buffer, 0);
    }

    void ColorBufferCommon::Bind(U32 slot) const
    {
        if (m_binding_type == eBindingStage::VertexShader)
            m_device_context->VSSetConstantBuffers(slot, 1, &m_color_buffer);

        if (m_binding_type == eBindingStage::PixelShader)
            m_device_context->PSSetConstantBuffers(slot, 1, &m_color_buffer);
    }

    void ColorBufferCommon::Shutdown()
    {
        if (m_color_buffer != nullptr)
        {
            m_color_buffer->Release();
            m_color_buffer = nullptr;
        }
    }
}
