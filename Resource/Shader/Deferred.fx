Texture2D shader_texture : register(t0);
SamplerState sample_type_wrap : register(s0);

cbuffer MatrixBuffer
{
    matrix model;
    matrix view;
    matrix projection;
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

PixelInputType DeferredVertexShader(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // Store the texture coordinates for the pixel shader.
    output.uv = input.uv;

    // Calculate the normal vector against the model matrix only.
    output.normal = mul(input.normal, (float3x3)model);

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}


PixelOutputType DeferredPixelShader(PixelInputType input) : SV_TARGET
{
    PixelOutputType output;

    // Sample the color from the texture and store it for output to the render target.
    output.color = shader_texture.Sample(sample_type_wrap, input.uv);

    // Store the normal for output to the render target.
    output.normal = float4(input.normal, 1.0f);

    return output;
}