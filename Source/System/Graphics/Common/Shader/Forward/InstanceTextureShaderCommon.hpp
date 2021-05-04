#pragma once
#include "../../../GraphicsAPI.hpp"
#include IncludeInstanceTextureShaderAPI
#include "../../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class MatrixData;
    class TextureCommon;
    class Color;
    class ShaderManagerCommon;
    class ShaderResource;
    class Matrix44;

    class InstanceTextureShaderCommon : public InstanceTextureShaderAPI
    {
    public:
        explicit InstanceTextureShaderCommon(ShaderManagerCommon* shader_manager);
        ~InstanceTextureShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 index_count, U32 instance_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void Shutdown();

    private:
        ShaderManagerCommon* m_shader_manager  = nullptr;
        ShaderResource*      m_shader_resource = nullptr;
    };

    
}
