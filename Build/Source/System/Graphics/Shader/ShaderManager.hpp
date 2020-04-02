#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../Renderer/RendererAPI.hpp"
#include IncludeShaderManagerAPI

namespace Engine5
{
    class MatrixData;
    class ResourceManager;
    class RendererCommon;
    class ColorShaderCommon;

    class ShaderManager : public ShaderManagerAPI
    {
    public:
        ShaderManager();
        ~ShaderManager();
        void Initialize(RendererCommon* renderer, ResourceManager* resource_manager);
        void Shutdown();

        void RenderColorShader(U32 indices_count, const MatrixData& mvp_data) const;

        ColorShaderCommon* GetColorShader() const;
    private:
        //shader list
        ColorShaderCommon* m_color_shader     = nullptr;
        ResourceManager*   m_resource_manager = nullptr;
    };
}
