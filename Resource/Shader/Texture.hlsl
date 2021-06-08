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
    float4 diffuse_texture = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular_texture = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);

    //sample diffuse texture
    if (diff_type == 0)
    {
        // sample simple diffuse texture.
        diffuse_texture = shader_texture[diff_id0].Sample(sample_type, input.tex);
    }
    else if (diff_type == 1)
    {
        // alpha mapping
        diffuse_texture = AlphaMapping(input.tex, diff_id0, diff_id1, diff_id2);
    }
    else if (diff_type == 2)
    {
        // light mapping
        diffuse_texture = LightMapping(input.tex, diff_id0, diff_id1);
    }
    else if (diff_type == 3)
    {
        // multiple texture blending
        diffuse_texture = BlendTexture(input.tex, diff_id0, diff_id1, gamma);
    }
    else
    {
        //use diffuse color
        diffuse_texture = diff_color;
    }

    if (spec_type == 1)
    {
        //specular mapping
        specular_texture = SpecularMapping(input.tex, spec_id0);
    }
    else
    {
        //use specular color
        specular_texture = spec_color;
    }

    //if (norm_type == 1)
    //{
    //    //use normal map
    //    normal = NormalMapping(input.tex, input.t, input.b, input.t, norm_id0);
    //}
    //else
    //{
    //    //use vertex normal
    //    normal = input.n;
    //}

 

    return input.color;
}