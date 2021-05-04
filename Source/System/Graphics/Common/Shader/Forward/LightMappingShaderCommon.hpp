#pragma once
#include "../../../GraphicsAPI.hpp"
#include "../../../../Core/Utility/CoreDef.hpp"

#include IncludeLightMappingShaderAPI

namespace Engine5
{
    class TextureArrayCommon;
    class Color;
    class TextureCommon;
    class MatrixData;
    class ShaderManagerCommon;
    class ShaderResource;

    class LightMappingShaderCommon : public LightMappingShaderAPI
    {
    public:
        explicit LightMappingShaderCommon(ShaderManagerCommon* shader_manager);
        ~LightMappingShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureArrayCommon* texture, const Color& color) const;
        void Shutdown();

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };

}
