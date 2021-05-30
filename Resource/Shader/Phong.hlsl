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
    float3 normal_world = normalize(input.normal);
    float3 to_eye_world = normalize(cam_pos - input.pos_world);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 A, D, S;

    for (int i = 0; i < light_count; ++i)
    {
        if (dynamic_light[i].type == 0)
        {
            CalculateAmbient(material, dynamic_light[i], ...);
        }
        else if (dynamic_light[i].type == 1)
        {
            CalculateDirectional(material, dynamic_light[i], ...);
        }
        else if (dynamic_light[i].type == 2)
        {
            CalculatePoint(material, dynamic_light[i], ...);
        }
        else if (dynamic_light[i].type == 3)
        {
            CalculateSpot(material, dynamic_light[i], ...);
        }
        else if (dynamic_light[i].type == 4)
        {
            CalculateCapsule(material, dynamic_light[i], ...);
        }

    }


    float4 final_color = ambient + diffuse + specular;
    // take alpha from diffuse materal
    final_color.a = material.diffuse.a; 

    //do other process.

    return final_color;
}
