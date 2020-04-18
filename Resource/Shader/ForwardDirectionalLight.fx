Texture2D shader_texture;
SamplerState sample_type;

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

cbuffer ColorBuffer
{
    float4 mesh_color;
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
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 world_position;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // Store the texture coordinates for the pixel shader.
    output.uv = input.uv;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)model);

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    world_position = mul(input.position, model);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.view_direction = camera_position.xyz - world_position.xyz;

    // Normalize the viewing direction vector.
    output.view_direction = normalize(output.view_direction);

    return output;
}


//pixel shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 texture_color;
    float3 light_dir;
    float  light_intensity;
    float4 color;
    float3 reflection;
    float4 specular;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    texture_color = mesh_color * shader_texture.Sample(sample_type, input.uv);

    color = ambient_color;

    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Invert the light direction for calculations.
    light_dir = -light_direction;

    // Calculate the amount of light on this pixel.
    light_intensity = saturate(dot(input.normal, light_dir));

    if (light_intensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuse_color * light_intensity);

        // Saturate the ambient and diffuse color.
        color = saturate(color);

        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * light_intensity * input.normal - light_dir);

        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.view_direction)), specular_power);
    }

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * texture_color;
    color = saturate(color + specular);
    return color;
}