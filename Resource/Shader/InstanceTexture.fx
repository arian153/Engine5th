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
    float4x4 world : WORLD;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
};

//instanced Vertex Shader
PixelInputType TextureVertexShader(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, input.world);
    output.position = mul(output.position, mvp);
    output.uv = input.uv;
    output.color = input.color;

    return output;
}


// Pixel Shader
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    return shader_texture.Sample(sample_type, input.uv) * color * input.color;
}
