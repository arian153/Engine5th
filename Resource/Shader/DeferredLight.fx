Texture2D color_texture : register(t0);
Texture2D normal_texture : register(t1);

SamplerState sample_type : register(s0);

cbuffer MatrixBuffer
{
    matrix model;
    matrix view;
    matrix projection;
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

    PixelInputType output;
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // Store the texture coordinates for the pixel shader.
    output.uv = input.uv;

    // Calculate the position of the vertex in the world.
    float4 world_position = mul(input.position, model);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.view_direction = camera_position.xyz - world_position.xyz;

    // Normalize the viewing direction vector.
    output.view_direction = normalize(output.view_direction);

    return output;
}


//pixel shader
float4 DeferredLightPixelShader(PixelInputType input) : SV_TARGET
{
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 colors = color_texture.Sample(sample_type, input.uv);

    // Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
    float4 normals = normal_texture.Sample(sample_type, input.uv);

    // Initialize the light color.
    float4 light_color = ambient_color;

    // Initialize the specular color.
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Invert the light direction for calculations.
    float3 light_dir = -light_direction;

    // Calculate the amount of light on this pixel.
    float light_intensity = saturate(dot(normals.xyz, light_dir));

    if (light_intensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        light_color += (diffuse_color * light_intensity);

        // Saturate the ambient and diffuse color.
        light_color = saturate(light_color);

        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        float3 reflection = normalize(2 * light_intensity * normals.xyz - light_dir);

        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.view_direction)), specular_power);
    }

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    light_color = light_color * colors;
    light_color = saturate(light_color + specular);

    return light_color;
}