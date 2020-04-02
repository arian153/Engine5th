#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include "../../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class LightShaderDX11
    {
    protected:
        struct MatrixBufferType
        {
            DirectX::XMMATRIX model;
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX projection;
        };

        struct CameraBufferType
        {
            DirectX::XMFLOAT3 camera_position;
            Real              padding;
        };

        struct LightBufferType
        {
            DirectX::XMFLOAT4 mesh_color;
            DirectX::XMFLOAT4 ambient_color;
            DirectX::XMFLOAT4 diffuse_color;
            DirectX::XMFLOAT3 light_direction;
            Real              specular_power;
            DirectX::XMFLOAT4 specular_color;
        };

    public:
        LightShaderDX11();
        ~LightShaderDX11();

        void SetHWnd(HWND hwnd);
        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);

    protected:
        ID3D11VertexShader*  m_vertex_shader  = nullptr;
        ID3D11PixelShader*   m_pixel_shader   = nullptr;
        ID3D11InputLayout*   m_layout         = nullptr;
        ID3D11Buffer*        m_matrix_buffer  = nullptr;
        ID3D11Buffer*        m_camera_buffer  = nullptr;
        ID3D11Buffer*        m_light_buffer   = nullptr;
        ID3D11SamplerState*  m_sampler_state  = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
        HWND                 m_hwnd           = nullptr;
    };
}
