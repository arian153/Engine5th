




float4 AlphaMapping(float2 tex, int diffuse1_id, int diffuse2_id int alpha_map_id)
{
    float4 diffuse1 = shader_texture[diffuse1_id].Sample(sample_type, tex);
    float4 diffuse2 = shader_texture[diffuse2_id].Sample(sample_type, tex);
    float4 alpha = shader_texture[alpha_map_id].Sample(sample_type, tex);

    float4 blend_color = (alpha * diffuse1) + ((1.0f - alpha) * diffuse2);
    return saturate(blend_color);
}

float3 NormalMapping(float2 tex, float3 t, float3 b, float3 n, int normal_map_id)
{
    float4 normal_map = shader_texture[normal_map_id].Sample(sample_type, tex);
    normal_map = (normal_map * 2.0f) - 1.0f;
    float3 normal = normal_map.x * t) + (normal_map.y * b) + (normal_map.z * n);
    return normalize(normal);
}

float4 SpecularMapping(float2 tex, int specular_map_id)
{
    return shader_texture[specular_map_id].Sample(sample_type, tex);
}

float3 LightMapping(float2 tex, int diffuse_id, int light_map_id)
{
    float4 diffuse = shader_texture[diffuse_id].Sample(sample_type, tex);
    float4 light_map = shader_texture[light_map_id].Sample(sample_type, tex);
    return diffuse * light_map;
}

float4 BlendTexture(float2 tex, int diffuse1_id, int diffuse2_id, float gamma)
{
    float4 diffuse1 = shader_texture[diffuse1_id].Sample(sample_type, tex);
    float4 diffuse2 = shader_texture[diffuse2_id].Sample(sample_type, tex);
    return diffuse1 * diffuse2 * gamma;
}

