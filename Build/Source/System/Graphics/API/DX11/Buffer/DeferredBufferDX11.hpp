#pragma once
#include <d3d11.h>
#include "../../../../Core/Utility/CoreDef.hpp"
#include "../../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Color;
    const U32 BUFFER_COUNT = 2;

    class DeferredBufferDX11
    {
    public:
        DeferredBufferDX11();
        ~DeferredBufferDX11();

        ID3D11ShaderResourceView* GetShaderResourceView(U32 view);

    protected:
        U32  m_texture_width  = 512;
        U32  m_texture_height = 512;
        Real m_screen_depth   = 1.0f;
        Real m_screen_near    = 0.0f;

        ID3D11Device*             m_device         = nullptr;
        ID3D11DeviceContext*      m_device_context = nullptr;
        ID3D11Texture2D*          m_render_target_texture_array[ BUFFER_COUNT ];
        ID3D11RenderTargetView*   m_render_target_view_array[ BUFFER_COUNT ];
        ID3D11ShaderResourceView* m_shader_resource_view_array[ BUFFER_COUNT ];
        ID3D11Texture2D*          m_depth_stencil_buffer = nullptr;
        ID3D11DepthStencilView*   m_depth_stencil_view   = nullptr;
        D3D11_VIEWPORT            m_viewport;
    };
}
