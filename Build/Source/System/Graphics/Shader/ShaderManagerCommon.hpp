#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../API/GraphicsAPI.hpp"
#include IncludeShaderManagerAPI

namespace Engine5
{
    class DeferredLightShaderCommon;
    class DirectionalLight;
    class Camera;
    class TextureCommon;
    class TextureShaderCommon;
    class MatrixData;
    class ResourceManager;
    class RendererCommon;
    class ColorShaderCommon;
    class DeferredShaderCommon;
    class DeferredBufferCommon;
    class Color;

    class ShaderManagerCommon : public ShaderManagerAPI
    {
    public:
        ShaderManagerCommon();
        ~ShaderManagerCommon();
        void Initialize(RendererCommon* renderer, ResourceManager* resource_manager);
        void Shutdown();

        void RenderColorShader(U32 indices_count, const MatrixData& mvp_data) const;
        void RenderTextureShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;
        void RenderDeferredLightShader(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const;
        void RenderDeferredShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;

        ColorShaderCommon*    GetColorShader() const;
        TextureShaderCommon*  GetTextureShader() const;
        DeferredLightShaderCommon*    GetDeferredLightShader() const;
        DeferredShaderCommon* GetDeferredShader() const;

    private:
        //shader list
        ColorShaderCommon*    m_color_shader     = nullptr;
        TextureShaderCommon*  m_texture_shader   = nullptr;
        DeferredLightShaderCommon*    m_light_shader     = nullptr;
        DeferredShaderCommon* m_deferred_shader  = nullptr;
        ResourceManager*      m_resource_manager = nullptr;
    };
}
