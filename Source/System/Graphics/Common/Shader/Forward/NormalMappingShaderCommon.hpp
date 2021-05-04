#pragma once
#include "../../../GraphicsAPI.hpp"
#include "../../../../Core/Utility/CoreDef.hpp"

#include IncludeNormalMappingShaderAPI

namespace Engine5
{
    class Camera;
    class DirectionalLight;
    class TextureArrayCommon;
    class Color;
    class TextureCommon;
    class MatrixData;
    class ShaderManagerCommon;
    class ShaderResource;

    class NormalMappingShaderCommon : public NormalMappingShaderAPI
    {
    public:
        explicit NormalMappingShaderCommon(ShaderManagerCommon* shader_manager);
        ~NormalMappingShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureArrayCommon* texture,  const Color& color, DirectionalLight* light) const;
        void Shutdown();

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };

    
}
