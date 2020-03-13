#pragma once
#include "../Renderer/RendererAPI.hpp"
#include IncludeColorShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class ShaderResource;
    class Matrix44;

    class ColorShaderCommon : public ColorShaderAPI
    {
    public:
        ColorShaderCommon();
        ~ColorShaderCommon();

        void SetVertexShader(ShaderResource* shader);
        void SetPixelShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const Matrix44& world, const Matrix44& view, const Matrix44& proj) const;
        void Shutdown();

    private:
        ShaderResource* m_vertex_shader_resource = nullptr;
        ShaderResource* m_pixel_shader_resource  = nullptr;
    };
}
