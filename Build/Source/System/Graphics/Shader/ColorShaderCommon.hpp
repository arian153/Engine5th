#pragma once
#include "../Renderer/RendererAPI.hpp"
#include IncludeColorShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class ShaderManager;
    class ShaderResource;
    class Matrix44;

    class ColorShaderCommon : public ColorShaderAPI
    {
    public:
        explicit ColorShaderCommon(ShaderManager* shader_manager);
        ~ColorShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const Matrix44& world, const Matrix44& view, const Matrix44& proj) const;
        void Shutdown();

    private:
        ShaderManager*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
