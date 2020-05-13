#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../API/GraphicsAPI.hpp"
#include IncludeShaderManagerAPI

namespace Engine5
{
    class InstanceTextureShaderCommon;
    class ForwardDirectionalLightShaderCommon;
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
        void RenderForwardDirectionalLightShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, Camera* camera, const Color& color, DirectionalLight* light) const;

        void RenderDeferredDirectionalLightShader(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const;
        void RenderDeferredBufferShader(U32 indices_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;

        void RenderInstanceTextureShader(U32 index_count, U32 instance_count, const MatrixData& mvp_data, TextureCommon* texture, const Color& color) const;

        ColorShaderCommon*                    GetColorShader() const;
        TextureShaderCommon*                  GetTextureShader() const;
        ForwardDirectionalLightShaderCommon*  GetForwardDirectionalLightShader() const;
        DeferredDirectionalLightShaderCommon* GetDeferredDirectionalShaderLightShader() const;
        DeferredBufferShaderCommon*           GetDeferredBufferShader() const;
        InstanceTextureShaderCommon* GetInstanceTextureShader() const;

    private:
        //shader list

        ColorShaderCommon*   m_color_shader   = nullptr;
        TextureShaderCommon* m_texture_shader = nullptr;
        InstanceTextureShaderCommon* m_instance_texture_shader = nullptr;

        //forward
        ForwardDirectionalLightShaderCommon* m_forward_directional_light_shader = nullptr;

        //deferred
        DeferredBufferShaderCommon*           m_deferred_buffer_shader            = nullptr;
        DeferredDirectionalLightShaderCommon* m_deferred_directional_light_shader = nullptr;

        //resource manager
        ResourceManager* m_resource_manager = nullptr;
    };
}
