#include "LightBufferDX11.hpp"

#include "../../Common/Buffer2/LightBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    LightBufferDX11::LightBufferDX11()
    {
    }

    LightBufferDX11::~LightBufferDX11()
    {
    }

    LightBufferCommon::LightBufferCommon()
    {
    }

    LightBufferCommon::~LightBufferCommon()
    {
    }

    bool LightBufferCommon::Init(RendererCommon* renderer, eBufferBindType type)
    {
        if (m_light_buffer != nullptr)
        {
            m_light_buffer->Release();
            m_light_buffer = nullptr;
        }

        m_binding_type = type;

        D3D11_BUFFER_DESC buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth           = sizeof(LightBufferType);
        buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags           = 0;
        buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_light_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void LightBufferCommon::Bind(U32 slot) const
    {
        if (m_binding_type == eBufferBindType::VertexShader)
            m_device_context->VSSetConstantBuffers(slot, 1, &m_light_buffer);

        if (m_binding_type == eBufferBindType::PixelShader)
            m_device_context->PSSetConstantBuffers(slot, 1, &m_light_buffer);
    }

    void LightBufferCommon::Shutdown()
    {
        if (m_light_buffer != nullptr)
        {
            m_light_buffer->Release();
            m_light_buffer = nullptr;
        }
    }
}
