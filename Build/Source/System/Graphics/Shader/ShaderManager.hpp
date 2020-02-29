#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../Renderer/APIMacro.hpp"
#include "../Renderer/DX11/Shader/ShaderManagerDX11.hpp"

namespace Engine5
{
    class ColorShaderCommon;

    class ShaderManager : public ShaderManagerAPI
    {
    public:
        ShaderManager();
        ~ShaderManager();
        void Initialize(RendererCommon* renderer);
        void Update(Real dt);
        void Shutdown();

        ColorShaderCommon* GetColorShader() const;

    private:
        //shader list
        ColorShaderCommon* m_color_shader = nullptr;
    };
}
