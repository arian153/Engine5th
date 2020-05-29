Texture2D shader_texture[2];
SamplerState sample_type;

cbuffer MatrixBuffer
{
    matrix mvp;
    matrix world;
};

cbuffer ColorBuffer
{
    float4 color;
};

cbuffer LightBuffer
{
    float4 diffuse_color;
    float3 light_direction;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

//Vertex Shader
PixelInputType TextureVertexShader(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3)world);
    output.normal = normalize(output.normal);

    output.tangent = mul(input.tangent, (float3x3)world);
    output.tangent = normalize(output.tangent);

    output.binormal = mul(input.binormal, (float3x3)world);
    output.binormal = normalize(output.binormal);

    return output;
}

// Pixel Shader
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 texture_color = shader_texture[ 0 ].Sample(sample_type, input.uv);
    float4 normal_map = shader_texture[ 1 ].Sample(sample_type, input.uv);
    normal_map = (normal_map * 2.0f) - 1.0f;

    normal_data = (normal_map.x * input.tangent) + (normal_map.y * input.binormal) + (normal_map.z * input.normal);
    normal_data = Normalize(normal_data);

    float3 light_dir = -light_direction;
    float light_intensity = saturate(dot(normal_data, light_dir));

    float4 blend_color = saturate(diffuse_color * light_intensity);
    return blend_color * color * texture_color;
}
