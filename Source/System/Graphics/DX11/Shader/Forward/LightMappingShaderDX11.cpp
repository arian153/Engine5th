#include "LightMappingShaderDX11.hpp"
#include "../../../Common/Shader/Forward/LightMappingShaderCommon.hpp"
#include <d3dcompiler.h>
#include "../../../Common/Shader/ShaderManagerCommon.hpp"
#include "../../../../../Manager/Resource/ResourceType/ShaderResource.hpp"
#include "../../ConverterDX11.hpp"
#include "../../../Common/Texture/TextureArrayCommon.hpp"
#include "../../../Common/DataType/MatrixData.hpp"

namespace Engine5
{
    LightMappingShaderDX11::LightMappingShaderDX11()
    {
    }

    LightMappingShaderDX11::~LightMappingShaderDX11()
    {
    }

    void LightMappingShaderDX11::SetHWnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void LightMappingShaderDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void LightMappingShaderDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    LightMappingShaderCommon::LightMappingShaderCommon(ShaderManagerCommon* shader_manager)
        : m_shader_manager(shader_manager)
    {
    }

    LightMappingShaderCommon::~LightMappingShaderCommon()
    {
    }

    void LightMappingShaderCommon::SetShader(ShaderResource* shader)
    {
        m_shader_resource = shader;
    }

    bool LightMappingShaderCommon::Initialize()
    {
        ID3D10Blob* error_message        = nullptr;
        ID3D10Blob* vertex_shader_buffer = nullptr;
        ID3D10Blob* pixel_shader_buffer  = nullptr;
        auto        vertex_shader_path   = m_shader_resource->WFilePath();
        auto        pixel_shader_path    = m_shader_resource->WFilePath();
        // Compile the vertex shader code.
        HRESULT result = D3DCompileFromFile(vertex_shader_path.c_str(), nullptr, nullptr, "VertexShaderEntry", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_message);
        if (FAILED(result))
        {
            // If the shader failed to compile it should have written something to the error message.
            if (error_message)
            {
                m_shader_manager->OutputShaderErrorMessage(error_message, m_hwnd, vertex_shader_path);
            }
                // If there was  nothing in the error message then it simply could not find the shader file itself.
            else
            {
                MessageBox(m_hwnd, vertex_shader_path.c_str(), L"Missing Shader File", MB_OK);
            }
            return false;
        }
        // Compile the pixel shader code.
        result = D3DCompileFromFile(pixel_shader_path.c_str(), nullptr, nullptr, "PixelShaderEntry", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixel_shader_buffer, &error_message);
        if (FAILED(result))
        {
            // If the shader failed to compile it should have written something to the error message.
            if (error_message)
            {
                m_shader_manager->OutputShaderErrorMessage(error_message, m_hwnd, pixel_shader_path);
            }
                // If there was nothing in the error message then it simply could not find the file itself.
            else
            {
                MessageBox(m_hwnd, pixel_shader_path.c_str(), L"Missing Shader File", MB_OK);
            }
            return false;
        }
        // Create the vertex shader from the buffer.
        result = m_device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), nullptr, &m_vertex_shader);
        if (FAILED(result))
        {
            return false;
        }
        // Create the pixel shader from the buffer.
        result = m_device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), nullptr, &m_pixel_shader);
        if (FAILED(result))
        {
            return false;
        }
        D3D11_INPUT_ELEMENT_DESC polygon_layout[ 2 ];
        // Create the vertex input layout description.
        // This setup needs to match the VertexType structure in the ModelClass and in the shader.
        polygon_layout[0].SemanticName         = "POSITION";
        polygon_layout[0].SemanticIndex        = 0;
        polygon_layout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
        polygon_layout[0].InputSlot            = 0;
        polygon_layout[0].AlignedByteOffset    = 0;
        polygon_layout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygon_layout[0].InstanceDataStepRate = 0;
        polygon_layout[1].SemanticName         = "TEXCOORD";
        polygon_layout[1].SemanticIndex        = 0;
        polygon_layout[1].Format               = DXGI_FORMAT_R32G32_FLOAT;
        polygon_layout[1].InputSlot            = 0;
        polygon_layout[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
        polygon_layout[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygon_layout[1].InstanceDataStepRate = 0;
        // Get a count of the elements in the layout.
        unsigned int num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);
        // Create the vertex input layout.
        result = m_device->CreateInputLayout(
                                             polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(),
                                             vertex_shader_buffer->GetBufferSize(), &m_layout);
        if (FAILED(result))
        {
            return false;
        }
        // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
        vertex_shader_buffer->Release();
        vertex_shader_buffer = nullptr;
        pixel_shader_buffer->Release();
        pixel_shader_buffer = nullptr;
        D3D11_BUFFER_DESC matrix_buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        matrix_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        matrix_buffer_desc.ByteWidth           = sizeof(MatrixBufferType);
        matrix_buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        matrix_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        matrix_buffer_desc.MiscFlags           = 0;
        matrix_buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        result = m_device->CreateBuffer(&matrix_buffer_desc, nullptr, &m_matrix_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Create a texture sampler state description.
        D3D11_SAMPLER_DESC sampler_desc;
        sampler_desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.MipLODBias     = 0.0f;
        sampler_desc.MaxAnisotropy  = 1;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampler_desc.BorderColor[0] = 0;
        sampler_desc.BorderColor[1] = 0;
        sampler_desc.BorderColor[2] = 0;
        sampler_desc.BorderColor[3] = 0;
        sampler_desc.MinLOD         = 0;
        sampler_desc.MaxLOD         = D3D11_FLOAT32_MAX;
        // Create the texture sampler state.
        result = m_device->CreateSamplerState(&sampler_desc, &m_sampler_state);
        if (FAILED(result))
        {
            return false;
        }
        // Setup the description of the dynamic color constant buffer that is in the pixel shader.
        D3D11_BUFFER_DESC color_buffer_desc;
        color_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        color_buffer_desc.ByteWidth           = sizeof(ColorBufferType);
        color_buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        color_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        color_buffer_desc.MiscFlags           = 0;
        color_buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        result = m_device->CreateBuffer(&color_buffer_desc, nullptr, &m_color_buffer);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    void LightMappingShaderCommon::Render(U32 indices_count, const MatrixData& mvp_data, TextureArrayCommon* texture, const Color& color) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        // Lock the constant buffer so it can be written to.
        HRESULT result = m_device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
        {
            return;
        }
        // Get a pointer to the data in the constant buffer.
        MatrixBufferType* matrix_data_ptr = (MatrixBufferType*)mapped_resource.pData;
        // Transpose the matrices to prepare them for the shader.
        // Copy the matrices into the constant buffer.
        matrix_data_ptr->mvp = XMMatrixTranspose(ConverterDX11::ToXMMatrix(mvp_data.GetMVPMatrix()));
        // Unlock the constant buffer.
        m_device_context->Unmap(m_matrix_buffer, 0);
        // Set the position of the constant buffer in the vertex shader.
        unsigned int buffer_number = 0;
        // Finally set the constant buffer in the vertex shader with the updated values.
        m_device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);
        // Set shader texture resource in the pixel shader
        U32 count = static_cast<U32>(texture->Size());
        m_device_context->PSSetShaderResources(0, count, texture->Data());
        // Color Buffer
        // Lock the light constant buffer so it can be written to.
        result = m_device_context->Map(m_color_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
        {
            return;
        }
        // Get a pointer to the data in the constant buffer.
        ColorBufferType* color_data_ptr = (ColorBufferType*)mapped_resource.pData;
        // Copy the lighting variables into the constant buffer.
        color_data_ptr->color = ConverterDX11::ToXMFloat4(color);
        // Unlock the constant buffer.
        m_device_context->Unmap(m_color_buffer, 0);
        // Set the position of the light constant buffer in the pixel shader.
        buffer_number = 0;
        // Finally set the light constant buffer in the pixel shader with the updated values.
        m_device_context->PSSetConstantBuffers(buffer_number, 1, &m_color_buffer);
        //render
        // Set the vertex input layout.
        m_device_context->IASetInputLayout(m_layout);
        // Set the vertex shader and pixel shader that will be used to render this triangle.
        m_device_context->VSSetShader(m_vertex_shader, nullptr, 0);
        m_device_context->PSSetShader(m_pixel_shader, nullptr, 0);
        // Set the sampler state in the pixel shader.
        m_device_context->PSSetSamplers(0, 1, &m_sampler_state);
        // Render the triangle.
        m_device_context->DrawIndexed(indices_count, 0, 0);
    }

    void LightMappingShaderCommon::Shutdown()
    {
        // Release the sampler state.
        if (m_sampler_state != nullptr)
        {
            m_sampler_state->Release();
            m_sampler_state = nullptr;
        }
        // Release the matrix constant buffer.
        if (m_matrix_buffer != nullptr)
        {
            m_matrix_buffer->Release();
            m_matrix_buffer = nullptr;
        }
        // Release the color constant buffer.
        if (m_color_buffer != nullptr)
        {
            m_color_buffer->Release();
            m_color_buffer = nullptr;
        }
        // Release the layout.
        if (m_layout != nullptr)
        {
            m_layout->Release();
            m_layout = nullptr;
        }
        // Release the pixel shader.
        if (m_pixel_shader != nullptr)
        {
            m_pixel_shader->Release();
            m_pixel_shader = nullptr;
        }
        // Release the vertex shader.
        if (m_vertex_shader != nullptr)
        {
            m_vertex_shader->Release();
            m_vertex_shader = nullptr;
        }
    }
}
