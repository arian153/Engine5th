#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../Renderer/RendererAPI.hpp"
#include IncludeShaderManagerAPI

namespace Engine5
{
    class ResourceManager;
    class RendererCommon;
    class ColorShaderCommon;
    class Matrix44;

    class ShaderManager : public ShaderManagerAPI
    {
    public:
        ShaderManager();
        ~ShaderManager();
        void Initialize(RendererCommon* renderer, ResourceManager* resource_manager);
        void Shutdown();

        void RenderColorShader(U32 indices_count, const Matrix44& world, const Matrix44& view, const Matrix44& proj) const;

        ColorShaderCommon* GetColorShader() const;
    private:
        //shader list
        ColorShaderCommon* m_color_shader     = nullptr;
        ResourceManager*   m_resource_manager = nullptr;
    };
}
