#pragma once
#include <d3d11.h>
#include "../../../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class Color;
    const U32 BUFFER_COUNT = 2;

    class DeferredBufferDX11
    {
    public:
        DeferredBufferDX11();
        ~DeferredBufferDX11();

        void                      SetDevice(ID3D11Device* device);
        void                      SetDeviceContext(ID3D11DeviceContext* device_context);
        bool                      BuildBuffer(U32 texture_width, U32 texture_height);
        ID3D11ShaderResourceView* GetShaderResourceView(U32 view);

    protected:
        U32  m_texture_width  = 512;
        U32  m_texture_height = 512;

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
