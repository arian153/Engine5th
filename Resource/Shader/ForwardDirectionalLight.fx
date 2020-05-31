Texture2D shader_texture;
SamplerState sample_type;

cbuffer MatrixBuffer
{
    matrix mvp;
    matrix model;
};

cbuffer CameraBuffer
{
    float3 camera_position;
    float padding;
};

cbuffer ColorBuffer
{
    float4 mesh_color;
};

cbuffer LightBuffer
{
    float4 ambient_color;
    float4 diffuse_color;
    float3 light_direction;
    float specular_power;
    float4 specular_color;
};

//defs
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
    float3 view_direction : TEXCOORD1;
};


//vertex shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;
    float4 world_position = mul(input.position, model);

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3)model);
    output.normal = normalize(output.normal);
    output.view_direction = camera_position.xyz - world_position.xyz;
    output.view_direction = normalize(output.view_direction);

    return output;
}


//pixel shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    float4 texture_color = mesh_color * shader_texture.Sample(sample_type, input.uv);
    float4 light_color = ambient_color;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 light_dir = -light_direction;
    float light_intensity = saturate(dot(input.normal, light_dir));

    if (light_intensity > 0.0f)
    {
        light_color += (diffuse_color * light_intensity);
        light_color = saturate(light_color);
        float3 reflection = normalize(2 * light_intensity * input.normal - light_dir);
        specular = pow(saturate(dot(reflection, input.view_direction)), specular_power);
    }

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    light_color = light_color * texture_color;
    light_color = saturate(light_color + specular);
    return light_color;
}