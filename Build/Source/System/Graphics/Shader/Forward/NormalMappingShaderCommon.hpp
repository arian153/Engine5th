#pragma once
#include "../../API/GraphicsAPI.hpp"
#include IncludeNormalMappingShaderAPI
#include "../../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
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
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureArrayCommon* texture, const Color& color, Real gamma = 2.2f) const;
        void Shutdown();

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
