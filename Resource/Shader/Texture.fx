Texture2D shader_texture;
SamplerState sample_type;

cbuffer MatrixBuffer
{
    matrix world_matrix;
    matrix view_matrix;
    matrix proj_matrix;
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
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, proj_matrix);

    // Store the texture coordinates for the pixel shader.
    output.uv = input.uv;

    return output;
}



// Pixel Shader
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 texture_color;

// Sample the pixel color from the texture using the sampler at this texture coordinate location.
texture_color = shader_texture.Sample(sample_type, input.uv);

return texture_color;
}
