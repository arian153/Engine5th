#include "DeferredDirectionalLightShaderDX11.hpp"
#include "../../../../Shader/Deferred/DeferredDirectionalLightShaderCommon.hpp"
#include "../../../../../Core/Utility/CoreUtility.hpp"
#include "../../ConverterDX11.hpp"
#include "../../../../../../Manager/Resource/ResourceType/ShaderResource.hpp"
#include "../../../../Shader/ShaderManagerCommon.hpp"
#include <d3dcompiler.h>
#include "../../../../Element/Camera.hpp"
#include "../../../../DataType/MatrixData.hpp"
#include "../../../../Light/DirectionalLight.hpp"
#include "../../../../Texture/TextureCommon.hpp"
#include "../../../../Buffer/DeferredBufferCommon.hpp"

namespace Engine5
{
    DeferredDirectionalLightShaderDX11::DeferredDirectionalLightShaderDX11()
    {
    }

    DeferredDirectionalLightShaderDX11::~DeferredDirectionalLightShaderDX11()
    {
    }

    void DeferredDirectionalLightShaderDX11::SetHWnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void DeferredDirectionalLightShaderDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void DeferredDirectionalLightShaderDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    DeferredDirectionalLightShaderCommon::DeferredDirectionalLightShaderCommon(ShaderManagerCommon* shader_manager)
        : m_shader_manager(shader_manager)
    {
    }

    DeferredDirectionalLightShaderCommon::~DeferredDirectionalLightShaderCommon()
    {
    }

    void DeferredDirectionalLightShaderCommon::SetShader(ShaderResource* shader)
    {
        m_shader_resource = shader;
    }

    bool DeferredDirectionalLightShaderCommon::Initialize()
    {
        ID3D10Blob* error_message        = nullptr;
        ID3D10Blob* vertex_shader_buffer = nullptr;
        ID3D10Blob* pixel_shader_buffer  = nullptr;
        auto        vertex_shader_path   = m_shader_resource->FilePath();
        auto        pixel_shader_path    = m_shader_resource->FilePath();
        // Compile the vertex shader code.
        HRESULT result = D3DCompileFromFile(vertex_shader_path.c_str(), nullptr, nullptr, "DeferredLightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_message);
        if (FAILED(result))
        {
            // If the shader failed to compile it should have written something to the error message.
            if (error_message)
            {
                m_shader_manager->OutputShaderErrorMessage(error_message, m_hwnd, vertex_shader_path);
            }
            else
            {
                // If there was  nothing in the error message then it simply could not find the shader file itself.
                MessageBox(m_hwnd, vertex_shader_path.c_str(), L"Missing Shader File", MB_OK);
            }
            return false;
        }
        // Compile the pixel shader code.
        result = D3DCompileFromFile(pixel_shader_path.c_str(), nullptr, nullptr, "DeferredLightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixel_shader_buffer, &error_message);
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
        //pos
        polygon_layout[0].SemanticName         = "POSITION";
        polygon_layout[0].SemanticIndex        = 0;
        polygon_layout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
        polygon_layout[0].InputSlot            = 0;
        polygon_layout[0].AlignedByteOffset    = 0;
        polygon_layout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygon_layout[0].InstanceDataStepRate = 0;
        //uv
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
        // Setup the description of the camera dynamic constant buffer that is in the vertex shader.
        D3D11_BUFFER_DESC camera_buffer_desc;
        camera_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        camera_buffer_desc.ByteWidth           = sizeof(CameraBufferType);
        camera_buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        camera_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        camera_buffer_desc.MiscFlags           = 0;
        camera_buffer_desc.StructureByteStride = 0;
        // Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        result = m_device->CreateBuffer(&camera_buffer_desc, nullptr, &m_camera_buffer);
        if (FAILED(result))
        {
            return false;
        }
        D3D11_BUFFER_DESC light_buffer_desc;
        light_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        light_buffer_desc.ByteWidth           = sizeof(LightBufferType);
        light_buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        light_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        light_buffer_desc.MiscFlags           = 0;
        light_buffer_desc.StructureByteStride = 0;
        result                                = m_device->CreateBuffer(&light_buffer_desc, nullptr, &m_light_buffer);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    void DeferredDirectionalLightShaderCommon::Render(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const
    {
        //
        // Set Vertex Shader constant buffer
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
        matrix_data_ptr->model = XMMatrixTranspose(ConverterDX11::ToXMMatrix(mvp_data.model));
        matrix_data_ptr->mvp   = XMMatrixTranspose(ConverterDX11::ToXMMatrix(mvp_data.GetMVPMatrix()));
        // Unlock the constant buffer.
        m_device_context->Unmap(m_matrix_buffer, 0);
        // Set the position of the constant buffer in the vertex shader.
        unsigned int buffer_number = 0;
        // Finally set the constant buffer in the vertex shader with the updated values.
        m_device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);
        result = m_device_context->Map(m_camera_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
        {
            return;
        }
        // Get a pointer to the data in the constant buffer.
        CameraBufferType* camera_data_ptr = (CameraBufferType*)mapped_resource.pData;
        // Copy the camera position into the constant buffer.
        camera_data_ptr->camera_position = ConverterDX11::ToXMFloat3(camera->GetPosition());
        camera_data_ptr->padding         = 0.0f;
        // Unlock the camera constant buffer.
        m_device_context->Unmap(m_camera_buffer, 0);
        // Set the position of the camera constant buffer in the vertex shader.
        buffer_number = 1;
        // Now set the camera constant buffer in the vertex shader with the updated values.
        m_device_context->VSSetConstantBuffers(buffer_number, 1, &m_camera_buffer);
        //
        // Set shader texture resource in the pixel shader.
        auto color_texture  = deferred_buffer->GetShaderResourceView(0);
        auto normal_texture = deferred_buffer->GetShaderResourceView(1);
        m_device_context->PSSetShaderResources(0, 1, &color_texture);
        m_device_context->PSSetShaderResources(1, 1, &normal_texture);
        //
        // Set Pixel Shader constant buffer
        // Lock the light constant buffer so it can be written to.
        result = m_device_context->Map(m_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
        {
            return;
        }
        // Get a pointer to the data in the constant buffer.
        LightBufferType* light_data_ptr = (LightBufferType*)mapped_resource.pData;
        // Copy the lighting variables into the constant buffer.
        light_data_ptr->ambient_color   = ConverterDX11::ToXMFloat4(light->ambient_color);
        light_data_ptr->diffuse_color   = ConverterDX11::ToXMFloat4(light->diffuse_color);
        light_data_ptr->specular_color  = ConverterDX11::ToXMFloat4(light->specular_color);
        light_data_ptr->light_direction = ConverterDX11::ToXMFloat3(light->direction);
        light_data_ptr->specular_power  = light->specular_power;
        // Unlock the constant buffer.
        m_device_context->Unmap(m_light_buffer, 0);
        // Set the position of the light constant buffer in the pixel shader.
        buffer_number = 0;
        // Finally set the light constant buffer in the pixel shader with the updated values.
        m_device_context->PSSetConstantBuffers(buffer_number, 1, &m_light_buffer);
        // Render Shader
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

    void DeferredDirectionalLightShaderCommon::Shutdown()
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
        if (m_camera_buffer != nullptr)
        {
            m_camera_buffer->Release();
            m_camera_buffer = nullptr;
        }
        if (m_light_buffer != nullptr)
        {
            m_light_buffer->Release();
            m_light_buffer = nullptr;
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
