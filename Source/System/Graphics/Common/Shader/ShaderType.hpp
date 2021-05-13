#pragma once

namespace Engine5
{
    enum class eShaderType
    {
        Invalid,
        Color,
        Texture,
        ForwardDirectionalLight,
        DeferredDirectionalLight,
        MultiTexture,
        AlphaMapping,
        LightMapping,
        NormalMapping,
        SpecularMapping
    };

    enum class eShaderStage : int
    {
        IntegratedShader = -1,
        VertexShader,
        PixelShader,
        InvalidShader
    };
}
