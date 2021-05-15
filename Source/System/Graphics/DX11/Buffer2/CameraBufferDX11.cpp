#include "CameraBufferDX11.hpp"

#include "../ConverterDX11.hpp"
#include "../../Common/Buffer2/CameraBufferCommon.hpp"
#include "../../Common/Element/Camera.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    CameraBufferDX11::CameraBufferDX11()
    {
    }

    CameraBufferDX11::~CameraBufferDX11()
    {
    }

    CameraBufferCommon::CameraBufferCommon()
    {
    }

    CameraBufferCommon::~CameraBufferCommon()
    {
    }

    bool CameraBufferCommon::Init(RendererCommon* renderer, eBindingStage type)
    {
        if (m_camera_buffer != nullptr)
        {
            m_camera_buffer->Release();
            m_camera_buffer = nullptr;
        }

        m_binding_type = type;

        D3D11_BUFFER_DESC buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth           = sizeof(CameraBufferType);
        buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags           = 0;
        buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_camera_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void CameraBufferCommon::Update(Camera* camera) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_camera_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;
        CameraBufferType* data_ptr = (CameraBufferType*)mapped_resource.pData;
        data_ptr->camera_position  = ConverterDX11::ToXMFloat3(camera->GetPosition());
        m_device_context->Unmap(m_camera_buffer, 0);
    }

    void CameraBufferCommon::Bind(U32 slot) const
    {
        if (m_binding_type == eBindingStage::VertexShader)
            m_device_context->VSSetConstantBuffers(slot, 1, &m_camera_buffer);

        if (m_binding_type == eBindingStage::PixelShader)
            m_device_context->PSSetConstantBuffers(slot, 1, &m_camera_buffer);
    }

    void CameraBufferCommon::Shutdown()
    {
        if (m_camera_buffer != nullptr)
        {
            m_camera_buffer->Release();
            m_camera_buffer = nullptr;
        }
    }
}
