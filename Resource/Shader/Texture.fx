Texture2D shader_texture;
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
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;

    return output;
}

// Pixel Shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    return shader_texture.Sample(sample_type, input.uv) * color;
}
