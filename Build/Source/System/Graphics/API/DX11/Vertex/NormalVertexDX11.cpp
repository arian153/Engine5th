#include "NormalVertexDX11.hpp"
#include "../../../../Math/Math.hpp"
#include "../ConverterDX11.hpp"
#include "../../../Vertex/TextureVertex.hpp"

namespace Engine5
{
    NormalVertexDX11::NormalVertexDX11()
        : position(), uv(), normal(), tangent(), binormal()
    {
    }

    NormalVertexDX11::~NormalVertexDX11()
    {
    }

    NormalVertex::NormalVertex()
    {
    }

    NormalVertex::NormalVertex(Real px, Real py, Real pz, Real u, Real v)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        uv.x       = u;
        uv.y       = v;
    }

    NormalVertex::NormalVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        uv.x       = u;
        uv.y       = v;
        normal.x   = nx;
        normal.y   = ny;
        normal.z   = nz;
    }

    NormalVertex::NormalVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        uv.x       = u;
        uv.y       = v;
        normal.x   = nx;
        normal.y   = ny;
        normal.z   = nz;
        tangent.x  = tx;
        tangent.y  = ty;
        tangent.z  = tz;
        binormal   = ConverterDX11::ToXMFloat3(CrossProduct(Vector3(tx, ty, tz), Vector3(nx, ny, nz)).Normalize());
    }

    NormalVertex::NormalVertex(const Vector3& p, const Vector2& _uv)
    {
        position = ConverterDX11::ToXMFloat3(p);
        uv       = ConverterDX11::ToXMFloat2(_uv);
    }

    NormalVertex::NormalVertex(const Vector3& p, const Vector2& _uv, const Vector3& n)
    {
        position = ConverterDX11::ToXMFloat3(p);
        uv       = ConverterDX11::ToXMFloat2(_uv);
        normal   = ConverterDX11::ToXMFloat3(n);
    }

    NormalVertex::NormalVertex(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t)
    {
        position = ConverterDX11::ToXMFloat3(p);
        uv       = ConverterDX11::ToXMFloat2(_uv);
        normal   = ConverterDX11::ToXMFloat3(n);
        tangent  = ConverterDX11::ToXMFloat3(t);
        binormal = ConverterDX11::ToXMFloat3(CrossProduct(t, n).Normalize());
    }

    NormalVertex::NormalVertex(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t, const Vector3& b)
    {
        position = ConverterDX11::ToXMFloat3(p);
        uv       = ConverterDX11::ToXMFloat2(_uv);
        normal   = ConverterDX11::ToXMFloat3(n);
        tangent  = ConverterDX11::ToXMFloat3(t);
        binormal = ConverterDX11::ToXMFloat3(b);
    }

    void NormalVertex::CalculateBinormal()
    {
        Vector3 n  = ConverterDX11::ToVector3(normal);
        Vector3 t  = ConverterDX11::ToVector3(tangent);
        Vector3 b  = CrossProduct(t, n).Normalize();
        binormal.x = b.x;
        binormal.y = b.y;
        binormal.z = b.z;
    }

    void NormalVertex::CalculateTangentAndBinormal()
    {
        Vector3 n = ConverterDX11::ToVector3(normal);
        Basis   basis;
        basis.CalculateBasisQuaternion(n);
        normal   = ConverterDX11::ToXMFloat3(basis.i);
        tangent  = ConverterDX11::ToXMFloat3(basis.j);
        binormal = ConverterDX11::ToXMFloat3(basis.k);
    }

    Vector3 NormalVertex::GetPosition() const
    {
        return ConverterDX11::ToVector3(position);
    }

    Vector2 NormalVertex::GetUV() const
    {
        return Vector2(uv.x, uv.y);
    }

    Vector3 NormalVertex::GetNormal() const
    {
        return ConverterDX11::ToVector3(normal);
    }

    Vector3 NormalVertex::GetTangent() const
    {
        return ConverterDX11::ToVector3(tangent);
    }

    Vector3 NormalVertex::GetBinormal() const
    {
        return ConverterDX11::ToVector3(binormal);
    }

    void NormalVertex::SetPosition(const Vector3& p)
    {
        position = ConverterDX11::ToXMFloat3(p);
    }

    void NormalVertex::SetUV(const Vector2& _uv)
    {
        uv = ConverterDX11::ToXMFloat2(_uv);
    }

    void NormalVertex::SetNormal(const Vector3& n)
    {
        normal = ConverterDX11::ToXMFloat3(n);
    }

    void NormalVertex::SetTangent(const Vector3& t)
    {
        tangent = ConverterDX11::ToXMFloat3(t);
    }

    void NormalVertex::SetBinormal(const Vector3& b)
    {
        binormal = ConverterDX11::ToXMFloat3(b);
    }
}
