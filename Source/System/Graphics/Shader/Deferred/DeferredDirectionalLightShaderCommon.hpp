#pragma once
#include "../../API/GraphicsAPI.hpp"
#include IncludeDeferredDirectionalLightShaderAPI
#include "../../../Core/Utility/CoreDef.hpp"

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

    class DeferredDirectionalLightShaderCommon : public DeferredDirectionalLightShaderAPI
    {
    public:
        explicit DeferredDirectionalLightShaderCommon(ShaderManagerCommon* shader_manager);
        ~DeferredDirectionalLightShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, DirectionalLight* light) const;
        void Shutdown();

    private:
        ShaderManagerCommon*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
