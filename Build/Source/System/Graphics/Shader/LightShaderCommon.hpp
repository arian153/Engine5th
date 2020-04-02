#pragma once
#include "../Renderer/RendererAPI.hpp"
#include IncludeLightShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class Camera;
    class ShaderManager;
    class ShaderResource;
    class Matrix44;

    class LightShaderCommon : public LightShaderAPI
    {
    public:
        explicit LightShaderCommon(ShaderManager* shader_manager);
        ~LightShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const Matrix44& world, const Matrix44& view, const Matrix44& proj, Camera* camera) const;
        void Shutdown();

    private:
        ShaderManager*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
