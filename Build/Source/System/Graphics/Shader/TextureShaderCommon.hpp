#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeTextureShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class MatrixData;
    class TextureCommon;
    class Color;
    class ShaderManager;
    class ShaderResource;
    class Matrix44;

    class TextureShaderCommon : public TextureShaderAPI
    {
    public:
        explicit TextureShaderCommon(ShaderManager* shader_manager);
        ~TextureShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void Shutdown();

    private:
        ShaderManager*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
