#pragma once
#include "../API/GraphicsAPI.hpp"
#include IncludeNormalVertexAPI
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Vector2;
    class Vector3;

    class NormalVertex : public NormalVertexAPI
    {
    public:
        NormalVertex();
        NormalVertex(Real px, Real py, Real pz, Real u, Real v);
        NormalVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz);
        NormalVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz);
        NormalVertex(const Vector3& p, const Vector2& _uv);
        NormalVertex(const Vector3& p, const Vector2& _uv, const Vector3& n);
        NormalVertex(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t);
        NormalVertex(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t, const Vector3& b);

        void CalculateBinormal();
        void CalculateTangentAndBinormal();

        Vector3 GetPosition() const;
        Vector2 GetUV() const;
        Vector3 GetNormal() const;
        Vector3 GetTangent() const;
        Vector3 GetBinormal() const;

        void SetPosition(const Vector3& p);
        void SetUV(const Vector2& _uv);
        void SetNormal(const Vector3& n);
        void SetTangent(const Vector3& t);
        void SetBinormal(const Vector3& b);

    public:
    };
}
