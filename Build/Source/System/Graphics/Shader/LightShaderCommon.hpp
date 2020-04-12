#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeLightShaderAPI
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

    class LightShaderCommon : public LightShaderAPI
    {
    public:
        explicit LightShaderCommon(ShaderManagerCommon* shader_manager);
        ~LightShaderCommon();

        void SetShader(ShaderResource* shader);

        bool Initialize();
        void Render(U32 indices_count, const MatrixData& mvp_data, DeferredBufferCommon* deferred_buffer, Camera* camera, const DirectionalLight& light) const;
        void Shutdown();

    private:
        ShaderManagerCommon*  m_shader_manager  = nullptr;
        ShaderResource* m_shader_resource = nullptr;
    };
}
