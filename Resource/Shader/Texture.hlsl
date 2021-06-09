Texture2D shader_texture[5];
SamplerState sample_type;

#include "TextureProcessing.hlsl"

//global
cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer TextureBuffer
{
    int diff_type;
    int diff_id0;
    int diff_id1;
    int diff_id2;
    int spec_type;
    int spec_id0;
    int norm_type;
    int norm_id0;
};

cbuffer MaterialBuffer
{
    float4 ambi_color;
    float4 diff_color;
    float4 spec_color;
    float shineness;
    float gamma;
    float2 padding_m;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

//vertex shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.tex = input.tex;

    return output;
}

//pixel shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    float4 diffuse_texture = ProcessDiffuse(input.tex, diff_type, 0, 1, 2, diff_color, gamma);
    
    return diffuse_texture;
}