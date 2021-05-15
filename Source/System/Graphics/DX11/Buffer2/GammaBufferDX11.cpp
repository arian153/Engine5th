#include "GammaBufferDX11.hpp"

#include "../../Common/Buffer2/GammaBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    GammaBufferDX11::GammaBufferDX11()
    {
    }

    GammaBufferDX11::~GammaBufferDX11()
    {
    }

    GammaBufferCommon::GammaBufferCommon()
    {
    }

    GammaBufferCommon::~GammaBufferCommon()
    {
    }

    bool GammaBufferCommon::Init(RendererCommon* renderer, eBindingStage type)
    {
        if (m_gamma_buffer != nullptr)
        {
            m_gamma_buffer->Release();
            m_gamma_buffer = nullptr;
        }

        m_binding_type = type;

        D3D11_BUFFER_DESC buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth           = sizeof(GammaBufferType);
        buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags           = 0;
        buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_gamma_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void GammaBufferCommon::Update(Real gamma) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_gamma_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;
        GammaBufferType* data_ptr  = (GammaBufferType*)mapped_resource.pData;
        data_ptr->gamma_correction = gamma;
        m_device_context->Unmap(m_gamma_buffer, 0);
    }

    void GammaBufferCommon::Bind(U32 slot) const
    {
        if (m_binding_type == eBindingStage::VertexShader)
            m_device_context->VSSetConstantBuffers(slot, 1, &m_gamma_buffer);

        if (m_binding_type == eBindingStage::PixelShader)
            m_device_context->PSSetConstantBuffers(slot, 1, &m_gamma_buffer);
    }

    void GammaBufferCommon::Shutdown()
    {
        if (m_gamma_buffer != nullptr)
        {
            m_gamma_buffer->Release();
            m_gamma_buffer = nullptr;
        }
    }
}
