#include "DirectionalLightShaderDX11.hpp"
#include "../../../../Shader/Forward/DirectionalLightShaderCommon.hpp"

namespace Engine5
{
    ForwardDirectionalLightShaderDX11::ForwardDirectionalLightShaderDX11()
    {
    }

    ForwardDirectionalLightShaderDX11::~ForwardDirectionalLightShaderDX11()
    {
    }

    void ForwardDirectionalLightShaderDX11::SetHWnd(HWND hwnd)
    {
    }

    void ForwardDirectionalLightShaderDX11::SetDevice(ID3D11Device* device)
    {
    }

    void ForwardDirectionalLightShaderDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
    }

    ForwardDirectionalLightShaderCommon::ForwardDirectionalLightShaderCommon(ShaderManagerCommon* shader_manager)
    {
    }

    ForwardDirectionalLightShaderCommon::~ForwardDirectionalLightShaderCommon()
    {
    }

    void ForwardDirectionalLightShaderCommon::SetShader(ShaderResource* shader)
    {
    }

    bool ForwardDirectionalLightShaderCommon::Initialize()
    {
        return true;
    }

    void ForwardDirectionalLightShaderCommon::Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const
    {
    }

    void ForwardDirectionalLightShaderCommon::Shutdown()
    {
    }
}
