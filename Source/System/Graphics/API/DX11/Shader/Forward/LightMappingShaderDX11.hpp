#pragma once
#include <d3d11.h>
#include <directxmath.h>

namespace Engine5
{
    class LightMappingShaderDX11
    {
    protected:
        struct MatrixBufferType
        {
            DirectX::XMMATRIX mvp;
        };

        struct ColorBufferType
        {
            DirectX::XMFLOAT4 color;
        };

    public:
        LightMappingShaderDX11();
        ~LightMappingShaderDX11();

        void SetHWnd(HWND hwnd);
        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);

    protected:
        ID3D11VertexShader*  m_vertex_shader  = nullptr;
        ID3D11PixelShader*   m_pixel_shader   = nullptr;
        ID3D11InputLayout*   m_layout         = nullptr;
        ID3D11Buffer*        m_matrix_buffer  = nullptr;
        ID3D11Buffer*        m_color_buffer   = nullptr;
        ID3D11SamplerState*  m_sampler_state  = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
        HWND                 m_hwnd           = nullptr;
    };
}
