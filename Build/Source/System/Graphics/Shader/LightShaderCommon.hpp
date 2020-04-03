#pragma once
#include "../Renderer/RendererAPI.hpp"
#include IncludeLightShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class Color;
    class MatrixData;
    class TextureCommon;
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
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, Camera* camera, const Color& color) const;
        void Shutdown();

    private:
        ShaderManager*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
