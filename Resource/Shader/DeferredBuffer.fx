Texture2D shader_texture : register(t0);
SamplerState sample_type_wrap : register(s0);

cbuffer MatrixBuffer
{
    matrix mvp;
    matrix model;
};

cbuffer ColorBuffer
{
    float4 mesh_color;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelOutputType
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
};

PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;
    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3)model);
    output.normal = normalize(output.normal);
    return output;
}


PixelOutputType PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    PixelOutputType output;
    output.color = mesh_color * shader_texture.Sample(sample_type_wrap, input.uv);
    output.normal = float4(input.normal, 1.0f);
    return output;
}