#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "../../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class ColorShaderDX11
    {
    protected:
        struct MatrixBufferType
        {
            DirectX::XMMATRIX world;
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX projection;
        };

    public:
        ColorShaderDX11();
        ~ColorShaderDX11();

        void SetHWnd(HWND hwnd);
        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);

    protected:
        void OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, const std::string& shader_name);

    protected:
        ID3D11VertexShader*  m_vertex_shader  = nullptr;
        ID3D11PixelShader*   m_pixel_shader   = nullptr;
        ID3D11InputLayout*   m_layout         = nullptr;
        ID3D11Buffer*        m_matrix_buffer  = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
        HWND                 m_hwnd           = nullptr;
    };
}
