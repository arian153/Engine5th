#include "LightHelper.hlsl"

Texture2D diffuse_map[2];
Texture2D specular_map;
Texture2D normal_map;

cbuffer MatrixBuffer
{
    matrix view;
    matrix proj;
};

cbuffer CameraBuffer
{
    float3 cam_pos;
};

cbuffer LightBuffer
{
    DynamicLight dynamic_light[16];
    int light_count;
};

struct VSIn
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 view_direction : TEXCOORD1;
};


VSOut VS(VSIn input)
{
    VSOut output;

    output.position = mul();

    return output;
}

float4 PS(VSOut input)
{
    float4 final_color;

    return final_color;
}
