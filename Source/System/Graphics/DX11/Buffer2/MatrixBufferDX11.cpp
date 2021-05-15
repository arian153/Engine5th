#include "MatrixBufferDX11.hpp"

#include "../ConverterDX11.hpp"
#include "../../Common/Buffer2/MatrixBufferCommon.hpp"
#include "../../Common/DataType/MatrixData.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace Engine5
{
    MatrixBufferDX11::MatrixBufferDX11()
    {
    }

    MatrixBufferDX11::~MatrixBufferDX11()
    {
    }

    MatrixBufferCommon::MatrixBufferCommon()
    {
    }

    MatrixBufferCommon::~MatrixBufferCommon()
    {
    }

    bool MatrixBufferCommon::Init(RendererCommon* renderer, eBindingStage type)
    {
        if (m_matrix_buffer != nullptr)
        {
            m_matrix_buffer->Release();
            m_matrix_buffer = nullptr;
        }

        m_binding_type = type;

        D3D11_BUFFER_DESC matrix_buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        matrix_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        matrix_buffer_desc.ByteWidth           = sizeof(MatrixBufferType);
        matrix_buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        matrix_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        matrix_buffer_desc.MiscFlags           = 0;
        matrix_buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&matrix_buffer_desc, nullptr, &m_matrix_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void MatrixBufferCommon::Update(const MatrixData& matrix_data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
            return;
        MatrixBufferType* data_ptr = (MatrixBufferType*)mapped_resource.pData;
        data_ptr->mvp              = DirectX::XMMatrixTranspose(ConverterDX11::ToXMMatrix(matrix_data.GetMVPMatrix()));
        data_ptr->world            = DirectX::XMMatrixTranspose(ConverterDX11::ToXMMatrix(matrix_data.model));
        m_device_context->Unmap(m_matrix_buffer, 0);
    }

    void MatrixBufferCommon::Bind(U32 slot) const
    {
        if (m_binding_type == eBindingStage::VertexShader)
            m_device_context->VSSetConstantBuffers(slot, 1, &m_matrix_buffer);

        if (m_binding_type == eBindingStage::PixelShader)
            m_device_context->PSSetConstantBuffers(slot, 1, &m_matrix_buffer);
    }

    void MatrixBufferCommon::Shutdown()
    {
        if (m_matrix_buffer != nullptr)
        {
            m_matrix_buffer->Release();
            m_matrix_buffer = nullptr;
        }
    }
}
