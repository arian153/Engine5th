#include "../../../DataType/Vertex/TextureVertex.hpp"
#include "TextureVertexDX11.hpp"
#include "../../../../Math/Math.hpp"
#include "../Converter.hpp"

namespace Engine5
{
    TextureVertexDX11::TextureVertexDX11()
        : position(), uv(), normal(), tangent(), binormal()
    {
    }

    TextureVertexDX11::~TextureVertexDX11()
    {
    }

    TextureVertex::TextureVertex()
    {
    }

    TextureVertex::TextureVertex(Real px, Real py, Real pz, Real u, Real v)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        uv.x       = u;
        uv.y       = v;
    }

    TextureVertex::TextureVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz)
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

    TextureVertex::TextureVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz)
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
        binormal   = Converter::ToXMFloat3(CrossProduct(Vector3(tx, ty, tz), Vector3(nx, ny, nz)).Normalize());
    }

    TextureVertex::TextureVertex(const Vector3& p, const Vector2& _uv)
    {
        position = Converter::ToXMFloat3(p);
        uv       = Converter::ToXMFloat2(_uv);
    }

    TextureVertex::TextureVertex(const Vector3& p, const Vector2& _uv, const Vector3& n)
    {
        position = Converter::ToXMFloat3(p);
        uv       = Converter::ToXMFloat2(_uv);
        normal   = Converter::ToXMFloat3(n);
    }

    TextureVertex::TextureVertex(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t)
    {
        position = Converter::ToXMFloat3(p);
        uv       = Converter::ToXMFloat2(_uv);
        normal   = Converter::ToXMFloat3(n);
        tangent  = Converter::ToXMFloat3(t);
        binormal = Converter::ToXMFloat3(CrossProduct(t, n).Normalize());
    }

    TextureVertex::TextureVertex(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t, const Vector3& b)
    {
        position = Converter::ToXMFloat3(p);
        uv       = Converter::ToXMFloat2(_uv);
        normal   = Converter::ToXMFloat3(n);
        tangent  = Converter::ToXMFloat3(t);
        binormal = Converter::ToXMFloat3(b);
    }

    void TextureVertex::CalculateBinormal()
    {
        Vector3 n  = Converter::ToVector3(normal);
        Vector3 t  = Converter::ToVector3(tangent);
        Vector3 b  = CrossProduct(t, n).Normalize();
        binormal.x = b.x;
        binormal.y = b.y;
        binormal.z = b.z;
    }

    void TextureVertex::CalculateTangentAndBinormal()
    {
        Vector3 n = Converter::ToVector3(normal);
        Basis   basis;
        basis.CalculateBasisQuaternion(n);
        normal   = Converter::ToXMFloat3(basis.i);
        tangent  = Converter::ToXMFloat3(basis.j);
        binormal = Converter::ToXMFloat3(basis.k);
    }

    Vector3 TextureVertex::GetPosition() const
    {
        return Converter::ToVector3(position);
    }

    Vector2 TextureVertex::GetUV() const
    {
        return Vector2(uv.x, uv.y);
    }

    Vector3 TextureVertex::GetNormal() const
    {
        return Converter::ToVector3(normal);
    }

    Vector3 TextureVertex::GetTangent() const
    {
        return Converter::ToVector3(tangent);
    }

    Vector3 TextureVertex::GetBinormal() const
    {
        return Converter::ToVector3(binormal);
    }

    void TextureVertex::SetPosition(const Vector3& p)
    {
        position = Converter::ToXMFloat3(p);
    }

    void TextureVertex::SetUV(const Vector2& _uv)
    {
        uv = Converter::ToXMFloat2(_uv);
    }

    void TextureVertex::SetNormal(const Vector3& n)
    {
        normal = Converter::ToXMFloat3(n);
    }

    void TextureVertex::SetTangent(const Vector3& t)
    {
        tangent = Converter::ToXMFloat3(t);
    }

    void TextureVertex::SetBinormal(const Vector3& b)
    {
        binormal = Converter::ToXMFloat3(b);
    }
}
