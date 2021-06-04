//global
cbuffer MatrixBuffer
{
    matrix view;
    matrix proj;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float4x4 world : WORLD;
    float4 ins_color : COLOR1;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

//vertex shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, input.world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.color = input.color * input.ins_color;

    return output;
}

//pixel shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    return input.color;
}
