Texture2D shader_texture[3];
SamplerState sample_type;

cbuffer MatrixBuffer
{
    matrix mvp;
};

cbuffer ColorBuffer
{
    float4 color;
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
    float4 alpha_value = shader_texture[ 2 ].Sample(sample_type, input.uv);
    float4 blend_color = (alpha_value * color1) + ((1.0f - alpha_value) * color2);
    blend_color = saturate(blend_color);
    return blend_color * color;
}
