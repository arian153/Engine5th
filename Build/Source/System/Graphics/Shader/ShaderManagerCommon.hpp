#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../API/GraphicsAPI.hpp"
#include IncludeShaderManagerAPI

namespace Engine5
{
    class DeferredDirectionalLightShaderCommon;
    class DirectionalLight;
    class Camera;
    class TextureCommon;
    class TextureShaderCommon;
    class MatrixData;
    class ResourceManager;
    class RendererCommon;
    class ColorShaderCommon;
    class DeferredBufferShaderCommon;
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
        void RenderDeferredDirectionalLightShader(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const;
        void RenderDeferredShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;

        ColorShaderCommon*    GetColorShader() const;
        TextureShaderCommon*  GetTextureShader() const;
        DeferredDirectionalLightShaderCommon*    GetDeferredLightShader() const;
        DeferredBufferShaderCommon* GetDeferredShader() const;

    private:
        //shader list
        ColorShaderCommon*    m_color_shader     = nullptr;
        TextureShaderCommon*  m_texture_shader   = nullptr;
        DeferredDirectionalLightShaderCommon*    m_light_shader     = nullptr;
        DeferredBufferShaderCommon* m_deferred_shader  = nullptr;
        ResourceManager*      m_resource_manager = nullptr;
    };
}
