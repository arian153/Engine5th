#include "ColorShader.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include <fstream>

namespace Engine5
{
    ColorShader::ColorShader(const std::string& vsr, const std::string& psr)
        : m_vsr(vsr), m_psr(psr)
    {
    }

    ColorShader::~ColorShader()
    {
    }

    void ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
    {
        InitializeShader(device, hwnd, m_vsr, m_psr);
    }

    void ColorShader::Update(Real dt, ID3D11DeviceContext* device_context, int index_count, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) const
    {
        E5_UNUSED_PARAM(dt);
        // Set the shader parameters that it will use for rendering.
        bool result = this->SetShaderParameters(device_context, world, view, proj);
        if (result == false)
        {
            return;
        }
        this->RenderShader(device_context, index_count);
    }

    void ColorShader::Shutdown()
    {
        ShutdownShader();
    }

    void ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, const std::string& vs_path, const std::string& ps_path)
    {
        // Initialize the pointers this function will use to null.
        ID3D10Blob* error_message        = nullptr;
        ID3D10Blob* vertex_shader_buffer = nullptr;
        ID3D10Blob* pixel_shader_buffer  = nullptr;

        // Compile the vertex shader code.
        HRESULT result = D3DCompileFromFile(StringToWString(vs_path).c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_message);
        if (FAILED(result))
        {
            // If the shader failed to compile it should have written something to the error message.
            if (error_message)
            {
                OutputShaderErrorMessage(error_message, hwnd, vs_path);
            }
                // If there was  nothing in the error message then it simply could not find the shader file itself.
            else
            {
                MessageBox(hwnd, StringToWString(vs_path).c_str(), L"Missing Shader File", MB_OK);
            }
            return;
        }

        // Compile the pixel shader code.
        result = D3DCompileFromFile(StringToWString(ps_path).c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixel_shader_buffer, &error_message);
        if (FAILED(result))
        {
            // If the shader failed to compile it should have written something to the error message.
            if (error_message)
            {
                OutputShaderErrorMessage(error_message, hwnd, ps_path);
            }
                // If there was nothing in the error message then it simply could not find the file itself.
            else
            {
                MessageBox(hwnd, StringToWString(ps_path).c_str(), L"Missing Shader File", MB_OK);
            }
            return;
        }

        // Create the vertex shader from the buffer.
        result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), nullptr, &m_vertex_shader);
        if (FAILED(result))
            return;

        // Create the pixel shader from the buffer.
        result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), nullptr, &m_pixel_shader);
        if (FAILED(result))
            return;
        D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
        // Create the vertex input layout description.
        // This setup needs to match the VertexType structure in the ModelClass and in the shader.
        polygon_layout[0].SemanticName         = "POSITION";
        polygon_layout[0].SemanticIndex        = 0;
        polygon_layout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
        polygon_layout[0].InputSlot            = 0;
        polygon_layout[0].AlignedByteOffset    = 0;
        polygon_layout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygon_layout[0].InstanceDataStepRate = 0;
        polygon_layout[1].SemanticName         = "COLOR";
        polygon_layout[1].SemanticIndex        = 0;
        polygon_layout[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
        polygon_layout[1].InputSlot            = 0;
        polygon_layout[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
        polygon_layout[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygon_layout[1].InstanceDataStepRate = 0;

        // Get a count of the elements in the layout.
        unsigned int num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

        // Create the vertex input layout.
        result = device->CreateInputLayout(
                                           polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(),
                                           vertex_shader_buffer->GetBufferSize(), &m_layout);
        if (FAILED(result))
            return;

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
        result = device->CreateBuffer(&matrix_buffer_desc, nullptr, &m_matrix_buffer);
        if (FAILED(result))
            return;
    }

    void ColorShader::ShutdownShader()
    {
        // Release the matrix constant buffer.
        if (m_matrix_buffer != nullptr)
        {
            m_matrix_buffer->Release();
            m_matrix_buffer = nullptr;
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

    void ColorShader::OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, const std::string& shader_name)
    {
        std::ofstream file_out;

        // Get a pointer to the error message text buffer.
        char* compile_errors = (char*)(error->GetBufferPointer());

        // Get the length of the message.
        unsigned long long buffer_size = error->GetBufferSize();

        // Open a file to write the error message to.
        file_out.open("shader-error.txt");

        // Write out the error message.
        for (unsigned long long i = 0; i < buffer_size; i++)
        {
            file_out << compile_errors[i];
        }

        // Close the file.
        file_out.close();

        // Release the error message.
        error->Release();
        error = nullptr;

        // Pop a message up on the screen to notify the user to check the text file for compile errors.
        MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", StringToWString(shader_name).c_str(), MB_OK);
    }

    bool ColorShader::SetShaderParameters(ID3D11DeviceContext* device_context, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        // Lock the constant buffer so it can be written to.
        HRESULT result = device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
            return false;

        // Get a pointer to the data in the constant buffer.
        MatrixBufferType* data_ptr = (MatrixBufferType*)mapped_resource.pData;

        // Transpose the matrices to prepare them for the shader.
        // Copy the matrices into the constant buffer.
        data_ptr->world      = XMMatrixTranspose(world);
        data_ptr->view       = XMMatrixTranspose(view);
        data_ptr->projection = XMMatrixTranspose(proj);

        // Unlock the constant buffer.
        device_context->Unmap(m_matrix_buffer, 0);

        // Set the position of the constant buffer in the vertex shader.
        unsigned int buffer_number = 0;

        // Finally set the constant buffer in the vertex shader with the updated values.
        device_context->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);
        return true;
    }

    void ColorShader::RenderShader(ID3D11DeviceContext* device_context, int index_count) const
    {
        // Set the vertex input layout.
        device_context->IASetInputLayout(m_layout);

        // Set the vertex shader and pixel shader that will be used to render this triangle.
        device_context->VSSetShader(m_vertex_shader, nullptr, 0);
        device_context->PSSetShader(m_pixel_shader, nullptr, 0);

        // Render the triangle.
        device_context->DrawIndexed(index_count, 0, 0);
    }
}
