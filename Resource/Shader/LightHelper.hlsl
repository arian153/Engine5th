struct DynamicLight
{
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float  pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float  Range;

    float3 Att;
    float  pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float  Range;

    float3 Direction;
    float  Spot;

    float3 Att;
    float  pad;
};

struct CapsuleLight
{

};

struct AmbientLight
{
    float3 AmbientDown;
    float3 AmbientRange;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};
