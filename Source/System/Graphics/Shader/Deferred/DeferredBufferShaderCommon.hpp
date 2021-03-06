#pragma once
#include "../../API/GraphicsAPI.hpp"
#include IncludeDeferredBufferShaderAPI
#include "../../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class TextureCommon;
    class Color;
    class MatrixData;
    class ShaderManagerCommon;
    class ShaderResource;
    class Matrix44;

    class DeferredBufferShaderCommon : public DeferredBufferShaderAPI
    {
    public:
        explicit DeferredBufferShaderCommon(ShaderManagerCommon* shader_manager);
        ~DeferredBufferShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void Shutdown();

    private:
        ShaderManagerCommon* m_shader_manager = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };

}
