#pragma once
#include <directxmath.h>
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Vector2;
    class Vector3;

    class Vertex
    {
    public:
        Vertex();
        Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv);
        Vertex(Real px, Real py, Real pz, Real nx, Real ny, Real nz, Real u, Real v);
        Vertex(Real px, Real py, Real pz, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz, Real u, Real v);
        Vertex(const Vector3& p, const Vector3& n, const Vector2& uv, const Vector3& t);

    public:
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT3 tangent;
        DirectX::XMFLOAT3 binormal;
    };
}
