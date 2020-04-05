//global
cbuffer MatrixBuffer
{
    matrix model;
    matrix view;
    matrix projection;
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
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // Store the input color for the pixel shader to use.
    output.color = input.color;

    return output;
}

//pixel shader
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
    return input.color;
}
