#pragma once
#include "../../../GraphicsAPI.hpp"
#include "../../../../Core/Utility/CoreDef.hpp"

#include IncludeForwardDirectionalLightShaderAPI

namespace Engine5
{
    class Camera;
    class DirectionalLight;
    class MatrixData;
    class TextureCommon;
    class Color;
    class ShaderManagerCommon;
    class ShaderResource;
    class Matrix44;

    class ForwardDirectionalLightShaderCommon : public ForwardDirectionalLightShaderAPI
    {
    public:
        explicit ForwardDirectionalLightShaderCommon(ShaderManagerCommon* shader_manager);
        ~ForwardDirectionalLightShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, Camera* camera, const Color& color, DirectionalLight* light) const;
        void Shutdown();

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };

    
}
