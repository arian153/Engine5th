#pragma once
#include "../../../Math/Utility/MathDef.hpp"
#include "../../Renderer/APIMacro.hpp"
#include IncludeTextureVertexAPI

namespace Engine5
{
    class Vector2;
    class Vector3;

    class TextureVertex : public TextureVertexAPI
    {
    public:
        TextureVertex();
        TextureVertex(Real px, Real py, Real pz, Real u, Real v);
        TextureVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz);
        TextureVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz);
        TextureVertex(const Vector3& p, const Vector2& uv);
        TextureVertex(const Vector3& p, const Vector2& uv, const Vector3& n);
        TextureVertex(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t);
        TextureVertex(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector3& b);

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
