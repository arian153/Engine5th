#include "ShaderManagerDX11.hpp"
#include "../../../Shader/ShaderManagerCommon.hpp"
#include "../../../Shader/Forward/ColorShaderCommon.hpp"
#include "../../../../../Manager/Resource/ResourceManager.hpp"
#include <fstream>
#include "../../../Shader/Forward/TextureShaderCommon.hpp"
#include "../../../Shader/Deferred/DeferredDirectionalLightShaderCommon.hpp"
#include "../../../Renderer/RendererCommon.hpp"
#include "../../../Shader/Deferred/DeferredBufferShaderCommon.hpp"
#include "../../../Shader/Forward/ForwardDirectionalLightShaderCommon.hpp"

namespace Engine5
{
    ShaderManagerDX11::ShaderManagerDX11()
    {
    }

    ShaderManagerDX11::~ShaderManagerDX11()
    {
    }

    void ShaderManagerDX11::SetHWnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void ShaderManagerDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void ShaderManagerDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    void ShaderManagerDX11::OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, const std::wstring& shader_name)
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
        MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shader_name.c_str(), MB_OK);
    }

    ShaderManagerCommon::ShaderManagerCommon()
    {
    }

    ShaderManagerCommon::~ShaderManagerCommon()
    {
    }

    void ShaderManagerCommon::Initialize(RendererCommon* renderer, ResourceManager* resource_manager)
    {
        SetHWnd(renderer->GetHwnd());
        SetDevice(renderer->GetDevice());
        SetDeviceContext(renderer->GetDeviceContext());
        m_resource_manager = resource_manager;
        //color shader
        m_color_shader = new ColorShaderCommon(this);
        m_color_shader->SetShader(m_resource_manager->GetShaderResourceFileName(L"Color.fx"));
        m_color_shader->SetHWnd(m_hwnd);
        m_color_shader->SetDevice(m_device);
        m_color_shader->SetDeviceContext(m_device_context);
        m_color_shader->Initialize();
        //texture shader
        m_texture_shader = new TextureShaderCommon(this);
        m_texture_shader->SetShader(m_resource_manager->GetShaderResourceFileName(L"Texture.fx"));
        m_texture_shader->SetHWnd(m_hwnd);
        m_texture_shader->SetDevice(m_device);
        m_texture_shader->SetDeviceContext(m_device_context);
        m_texture_shader->Initialize();
        //Deferred Directional Light shader
        m_forward_directional_light_shader = new ForwardDirectionalLightShaderCommon(this);
        m_forward_directional_light_shader->SetShader(m_resource_manager->GetShaderResourceFileName(L"ForwardDirectionalLight.fx"));
        m_forward_directional_light_shader->SetHWnd(m_hwnd);
        m_forward_directional_light_shader->SetDevice(m_device);
        m_forward_directional_light_shader->SetDeviceContext(m_device_context);
        m_forward_directional_light_shader->Initialize();
        //Deferred Directional Light shader
        m_deferred_directional_light_shader = new DeferredDirectionalLightShaderCommon(this);
        m_deferred_directional_light_shader->SetShader(m_resource_manager->GetShaderResourceFileName(L"DeferredDirectionalLight.fx"));
        m_deferred_directional_light_shader->SetHWnd(m_hwnd);
        m_deferred_directional_light_shader->SetDevice(m_device);
        m_deferred_directional_light_shader->SetDeviceContext(m_device_context);
        m_deferred_directional_light_shader->Initialize();
        //Deferred Buffer shader
        m_deferred_buffer_shader = new DeferredBufferShaderCommon(this);
        m_deferred_buffer_shader->SetShader(m_resource_manager->GetShaderResourceFileName(L"DeferredBuffer.fx"));
        m_deferred_buffer_shader->SetHWnd(m_hwnd);
        m_deferred_buffer_shader->SetDevice(m_device);
        m_deferred_buffer_shader->SetDeviceContext(m_device_context);
        m_deferred_buffer_shader->Initialize();
    }

    void ShaderManagerCommon::Shutdown()
    {
        if (m_color_shader != nullptr)
        {
            m_color_shader->Shutdown();
            delete m_color_shader;
            m_color_shader = nullptr;
        }
        if (m_texture_shader != nullptr)
        {
            m_texture_shader->Shutdown();
            delete m_texture_shader;
            m_texture_shader = nullptr;
        }
        if (m_forward_directional_light_shader != nullptr)
        {
            m_forward_directional_light_shader->Shutdown();
            delete m_forward_directional_light_shader;
            m_forward_directional_light_shader = nullptr;
        }
        if (m_deferred_directional_light_shader != nullptr)
        {
            m_deferred_directional_light_shader->Shutdown();
            delete m_deferred_directional_light_shader;
            m_deferred_directional_light_shader = nullptr;
        }
        if (m_deferred_buffer_shader != nullptr)
        {
            m_deferred_buffer_shader->Shutdown();
            delete m_deferred_buffer_shader;
            m_deferred_buffer_shader = nullptr;
        }
    }

    void ShaderManagerCommon::RenderColorShader(U32 indices_count, const MatrixData& mvp_data) const
    {
        m_color_shader->Render(indices_count, mvp_data);
    }

    void ShaderManagerCommon::RenderTextureShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const
    {
        m_texture_shader->Render(indices_count, mvp_data, texture, color);
    }

    void ShaderManagerCommon::RenderForwardDirectionalLightShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, Camera* camera, const Color& color, DirectionalLight* light) const
    {
        m_forward_directional_light_shader->Render(indices_count, mvp_data, texture, camera, color, light);
    }

    void ShaderManagerCommon::RenderDeferredDirectionalLightShader(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const
    {
        m_deferred_directional_light_shader->Render(indices_count, mvp_data, deferred_buffer, camera, light);
    }

    void ShaderManagerCommon::RenderDeferredBufferShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const
    {
        m_deferred_buffer_shader->Render(indices_count, mvp_data, texture, color);
    }

    ColorShaderCommon* ShaderManagerCommon::GetColorShader() const
    {
        return m_color_shader;
    }

    TextureShaderCommon* ShaderManagerCommon::GetTextureShader() const
    {
        return m_texture_shader;
    }

    ForwardDirectionalLightShaderCommon* ShaderManagerCommon::GetForwardDirectionalLightShader() const
    {
        return m_forward_directional_light_shader;
    }

    DeferredDirectionalLightShaderCommon* ShaderManagerCommon::GetDeferredDirectionalShaderLightShader() const
    {
        return m_deferred_directional_light_shader;
    }

    DeferredBufferShaderCommon* ShaderManagerCommon::GetDeferredBufferShader() const
    {
        return m_deferred_buffer_shader;
    }
}
