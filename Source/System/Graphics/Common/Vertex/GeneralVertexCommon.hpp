#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeGeneralVertexAPI

namespace Engine5
{
    class Vector2;
    class Vector3;

    class GeneralVertexCommon : public GeneralVertexAPI
    {
    public:
        GeneralVertexCommon();
        GeneralVertexCommon(const Vector3& p, const Vector2& uv);
        GeneralVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n);
        GeneralVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t);

        Vector3 GetPosition() const;
        Vector2 GetTex() const;
        Vector3 GetNormal() const;
        Vector3 GetTangent() const;

        void SetPosition(const Vector3& p);
        void SetTex(const Vector2& uv);
        void SetNormal(const Vector3& n);
        void SetTangent(const Vector3& t);

    public:
    };
}
