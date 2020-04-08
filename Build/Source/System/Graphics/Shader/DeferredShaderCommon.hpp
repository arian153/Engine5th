#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeDeferredShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class TextureCommon;
    class Color;
    class MatrixData;
    class ShaderManager;
    class ShaderResource;
    class Matrix44;

    class DeferredShaderCommon : public DeferredShaderAPI
    {
    public:
        explicit DeferredShaderCommon(ShaderManager* shader_manager);
        ~DeferredShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void Shutdown();

    private:
        ShaderManager* m_shader_manager = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };

}
