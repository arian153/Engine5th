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

};

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
    float3 view_direction : TEXCOORD1;
};

