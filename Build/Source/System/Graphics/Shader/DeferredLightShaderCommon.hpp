#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeDeferredLightShaderAPI
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class DeferredBufferCommon;
    class DirectionalLight;
    class Color;
    class MatrixData;
    class TextureCommon;
    class Camera;
    class ShaderManagerCommon;
    class ShaderResource;
    class Matrix44;

    class DeferredLightShaderCommon : public DeferredLightShaderAPI
    {
    public:
        explicit DeferredLightShaderCommon(ShaderManagerCommon* shader_manager);
        ~DeferredLightShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const;
        void Shutdown();

    private:
        ShaderManagerCommon*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
