#include "DeferredBufferDX11.hpp"
#include "../../../../Core/Utility/CoreDef.hpp"
#include "../../../../Math/Utility/MathDef.hpp"
#include "../../../DataType/Color.hpp"
#include "../../../Buffer/DeferredBufferCommon.hpp"

namespace Engine5
{
    DeferredBufferDX11::DeferredBufferDX11()
        : m_render_target_texture_array{}, m_render_target_view_array{}, m_shader_resource_view_array{}, m_viewport()
    {
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            m_render_target_texture_array[i] = nullptr;
            m_render_target_view_array[i]    = nullptr;
            m_shader_resource_view_array[i]  = nullptr;
        }
    }

    DeferredBufferDX11::~DeferredBufferDX11()
    {
    }

    ID3D11ShaderResourceView* DeferredBufferDX11::GetShaderResourceView(U32 view)
    {
        return m_shader_resource_view_array[view];
    }

    DeferredBufferCommon::DeferredBufferCommon()
    {
    }

    DeferredBufferCommon::~DeferredBufferCommon()
    {
    }

    bool DeferredBufferCommon::Initialize(int texture_width, int texture_height, Real screen_depth, Real screen_near)
    {
        // Store the width and height of the render texture.
        m_texture_width  = texture_width;
        m_texture_height = texture_height;
        m_screen_depth   = screen_depth;
        m_screen_near    = screen_near;
        // Initialize the render target texture description.
        D3D11_TEXTURE2D_DESC texture_desc;
        ZeroMemory(&texture_desc, sizeof(texture_desc));
        // Setup the render target texture description.
        texture_desc.Width            = texture_width;
        texture_desc.Height           = texture_height;
        texture_desc.MipLevels        = 1;
        texture_desc.ArraySize        = 1;
        texture_desc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.Usage            = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags   = 0;
        texture_desc.MiscFlags        = 0;
        // Create the render target textures.
        HRESULT result;
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            result = m_device->CreateTexture2D(&texture_desc, nullptr, &m_render_target_texture_array[i]);
            if (FAILED(result))
            {
                return false;
            }
        }
        // Setup the description of the render target view.
        D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
        render_target_view_desc.Format             = texture_desc.Format;
        render_target_view_desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
        render_target_view_desc.Texture2D.MipSlice = 0;
        // Create the render target views.
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            result = m_device->CreateRenderTargetView(m_render_target_texture_array[i], &render_target_view_desc, &m_render_target_view_array[i]);
            if (FAILED(result))
            {
                return false;
            }
        }
        // Setup the description of the shader resource view.
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
        shader_resource_view_desc.Format                    = texture_desc.Format;
        shader_resource_view_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
        shader_resource_view_desc.Texture2D.MipLevels       = 1;
        // Create the shader resource views.
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            result = m_device->CreateShaderResourceView(m_render_target_texture_array[i], &shader_resource_view_desc, &m_shader_resource_view_array[i]);
            if (FAILED(result))
            {
                return false;
            }
        }
        // Initialize the description of the depth buffer.
        D3D11_TEXTURE2D_DESC depth_buffer_desc;
        ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
        // Set up the description of the depth buffer.
        depth_buffer_desc.Width              = texture_width;
        depth_buffer_desc.Height             = texture_height;
        depth_buffer_desc.MipLevels          = 1;
        depth_buffer_desc.ArraySize          = 1;
        depth_buffer_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count   = 1;
        depth_buffer_desc.SampleDesc.Quality = 0;
        depth_buffer_desc.Usage              = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags     = 0;
        depth_buffer_desc.MiscFlags          = 0;
        // Create the texture for the depth buffer using the filled out description.
        result = m_device->CreateTexture2D(&depth_buffer_desc, nullptr, &m_depth_stencil_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Initialize the depth stencil view description.
        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
        ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
        // Set up the depth stencil view description.
        depth_stencil_view_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_stencil_view_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        depth_stencil_view_desc.Texture2D.MipSlice = 0;
        // Create the depth stencil view.
        result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
        if (FAILED(result))
        {
            return false;
        }
        // Setup the viewport for rendering.
        m_viewport.Width    = (Real)texture_width;
        m_viewport.Height   = (Real)texture_height;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;
        return true;
    }

    void DeferredBufferCommon::Shutdown()
    {
        if (m_depth_stencil_view)
        {
            m_depth_stencil_view->Release();
            m_depth_stencil_view = nullptr;
        }
        if (m_depth_stencil_buffer)
        {
            m_depth_stencil_buffer->Release();
            m_depth_stencil_buffer = nullptr;
        }
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            if (m_shader_resource_view_array[i])
            {
                m_shader_resource_view_array[i]->Release();
                m_shader_resource_view_array[i] = nullptr;
            }
            if (m_render_target_view_array[i])
            {
                m_render_target_view_array[i]->Release();
                m_render_target_view_array[i] = nullptr;
            }
            if (m_render_target_texture_array[i])
            {
                m_render_target_texture_array[i]->Release();
                m_render_target_texture_array[i] = nullptr;
            }
        }
    }

    void DeferredBufferCommon::SetRenderTargets() const
    {
        // Bind the render target view array and depth stencil buffer to the output render pipeline.
        m_device_context->OMSetRenderTargets(BUFFER_COUNT, m_render_target_view_array, m_depth_stencil_view);
        // Set the viewport.
        m_device_context->RSSetViewports(1, &m_viewport);
    }

    void DeferredBufferCommon::ClearRenderTargets(const Color& color)
    {
        Real color_array[ 4 ];
        // Setup the color to clear the buffer to.
        color_array[0] = color.r;
        color_array[1] = color.g;
        color_array[2] = color.b;
        color_array[3] = color.a;
        // Clear the render target buffers.
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            m_device_context->ClearRenderTargetView(m_render_target_view_array[i], color_array);
        }
        // Clear the depth buffer.
        m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }
}
