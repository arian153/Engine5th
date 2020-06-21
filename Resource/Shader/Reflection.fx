Texture2D shader_texture;
SamplerState sample_type;
Texture2D reflection_texture;

cbuffer MatrixBuffer
{
    matrix mvp;
    matrix proj;
    matrix world;
};

cbuffer ReflectionBuffer
{
    matrix reflection_matrix;
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
    float4 reflection_position : TEXCOORD1;
};

//Vertex Shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    matrix reflect_project_world = mul(reflection_matrix, proj);
    reflect_project_world = mul(world, reflect_project_world);

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;
    output.reflection_position = mul(input.position, reflect_project_world);

    return output;
}

// Pixel Shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    float4 texture_color = shader_texture.Sample(sample_type, input.uv);
    float2 reflect_uv;
    reflect_uv.x = input.reflection_position.x / input.reflection_position.w / 2.0f + 0.5f;
    reflect_uv.y = -input.reflection_position.y / input.reflection_position.w / 2.0f + 0.5f;
    float4 reflection_color = reflection_texture.Sample(sample_type, reflect_uv);
    float4 final_color = lerp(texture_color, reflection_color, 0.15f) * color;
    return final_color;
}
