#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class ColorShader
    {
    private:
        struct MatrixBufferType
        {
            DirectX::XMMATRIX world;
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX projection;
        };

    public:
        ColorShader(const std::string& vsr, const std::string& psr);
        ~ColorShader();

        void Initialize(ID3D11Device* device, HWND hwnd);
        void Update(Real dt, ID3D11DeviceContext* device_context, int index_count, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) const;
        void Shutdown();

    private:
        void InitializeShader(ID3D11Device* device, HWND hwnd, const std::string& vs_path, const std::string& ps_path);
        void ShutdownShader();
        void OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, const std::string& shader_name);

        bool SetShaderParameters(ID3D11DeviceContext* device_context, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) const;
        void RenderShader(ID3D11DeviceContext* device_context, int index_count) const;

    private:
        ID3D11VertexShader* m_vertex_shader = nullptr;
        ID3D11PixelShader*  m_pixel_shader  = nullptr;
        ID3D11InputLayout*  m_layout        = nullptr;
        ID3D11Buffer*       m_matrix_buffer = nullptr;

        std::string m_vsr;
        std::string m_psr;
    };
}
