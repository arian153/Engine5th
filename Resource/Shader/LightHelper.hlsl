struct DynamicLight
{
    float4 AmbientDown;
    float4 AmbientRange;

};

struct DirectionalLight
{
    float4 AmbientDown;
    float4 AmbientRange;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float  pad;
};

struct PointLight
{
    float4 AmbientDown;
    float4 AmbientRange;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float  Range;

    float3 Att;
    float  pad;
};

struct SpotLight
{
    float4 AmbientDown;
    float4 AmbientRange;
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
    float4 AmbientDown;
    float4 AmbientRange;
    float4 Diffuse;
    float4 Specular;

    float3 pos;
    float range;

    float3 dir;
    float len;

};


struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};
