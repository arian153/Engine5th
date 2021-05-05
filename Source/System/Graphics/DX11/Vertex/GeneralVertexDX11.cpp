#include "GeneralVertexDX11.hpp"

#include "../ConverterDX11.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../Common/Vertex/GeneralVertexCommon.hpp"

namespace Engine5
{
    GeneralVertexDX11::GeneralVertexDX11()
        : position(), tex(), normal(), tangent()
    {
    }

    GeneralVertexDX11::~GeneralVertexDX11()
    {
    }

    GeneralVertexCommon::GeneralVertexCommon()
    {
    }

    GeneralVertexCommon::GeneralVertexCommon(const Vector3& p, const Vector2& uv)
    {
        position = ConverterDX11::ToXMFloat3(p);
        tex      = ConverterDX11::ToXMFloat2(uv);
    }

    GeneralVertexCommon::GeneralVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n)
    {
        position = ConverterDX11::ToXMFloat3(p);
        tex      = ConverterDX11::ToXMFloat2(uv);
        normal   = ConverterDX11::ToXMFloat3(n);
    }

    GeneralVertexCommon::GeneralVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t)
    {
        position = ConverterDX11::ToXMFloat3(p);
        tex      = ConverterDX11::ToXMFloat2(uv);
        normal   = ConverterDX11::ToXMFloat3(n);
        tangent  = ConverterDX11::ToXMFloat3(t);
    }

    Vector3 GeneralVertexCommon::GetPosition() const
    {
        return ConverterDX11::ToVector3(position);
    }

    Vector2 GeneralVertexCommon::GetTex() const
    {
        return Vector2(tex.x, tex.y);
    }

    Vector3 GeneralVertexCommon::GetNormal() const
    {
        return ConverterDX11::ToVector3(normal);
    }

    Vector3 GeneralVertexCommon::GetTangent() const
    {
        return ConverterDX11::ToVector3(tangent);
    }

    void GeneralVertexCommon::SetPosition(const Vector3& p)
    {
        position = ConverterDX11::ToXMFloat3(p);
    }

    void GeneralVertexCommon::SetTex(const Vector2& uv)
    {
        tex = ConverterDX11::ToXMFloat2(uv);
    }

    void GeneralVertexCommon::SetNormal(const Vector3& n)
    {
        normal = ConverterDX11::ToXMFloat3(n);
    }

    void GeneralVertexCommon::SetTangent(const Vector3& t)
    {
        tangent = ConverterDX11::ToXMFloat3(t);
    }
}
