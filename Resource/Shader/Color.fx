//global
cbuffer MatrixBuffer
{
    matrix mvp;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
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
    output.position = mul(input.position, mvp);
    output.color = input.color;

    return output;
}

//pixel shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    return input.color;
}
