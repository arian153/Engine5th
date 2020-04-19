Texture2D color_texture : register(t0);
Texture2D normal_texture : register(t1);

SamplerState sample_type : register(s0);

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


cbuffer LightBuffer
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;
    float3 light_direction;
    float specular_power;
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
    float3 view_direction : TEXCOORD1;
};


//vertex shader
PixelInputType DeferredLightVertexShader(VertexInputType input)
{
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    float4 world_position = mul(input.position, model);

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;
    output.view_direction = camera_position.xyz - world_position.xyz;
    output.view_direction = normalize(output.view_direction);

    return output;
}


//pixel shader
float4 DeferredLightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 colors = color_texture.Sample(sample_type, input.uv);
    float4 normals = normal_texture.Sample(sample_type, input.uv);
    float4 light_color = ambient_color;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 light_dir = -light_direction;
    float light_intensity = saturate(dot(normals.xyz, light_dir));

    if (light_intensity > 0.0f)
    {
        light_color += (diffuse_color * light_intensity);
        light_color = saturate(light_color);
        float3 reflection = normalize(2 * light_intensity * normals.xyz - light_dir);
        specular = pow(saturate(dot(reflection, input.view_direction)), specular_power);
    }

    light_color = light_color * colors;
    light_color = saturate(light_color + specular);

    return light_color;
}