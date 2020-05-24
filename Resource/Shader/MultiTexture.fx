Texture2D shader_texture[2];
SamplerState sample_type;

cbuffer MatrixBuffer
{
    matrix mvp;
};

cbuffer ColorBuffer
{
    float4 color;
};

cbuffer GammaBuffer
{
    float gamma_correction;
    float3 padding;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

//Vertex Shader
PixelInputType TextureVertexShader(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;

    return output;
}

// Pixel Shader
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 color1 = shader_texture[ 0 ].Sample(sample_type, input.uv);
    float4 color2 = shader_texture[ 1 ].Sample(sample_type, input.uv);
    float4 blend_color = color1 * color2 * gamma_correction;
    blend_color = saturate(blend_color);
    return blend_color * color;
}
