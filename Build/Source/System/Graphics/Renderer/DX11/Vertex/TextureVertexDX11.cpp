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
        binormal   = Converter::ToXMFloat3(CrossProduct(Vector3(tx, ty, tz), Vector3(nx, ny, nz)).Unit());
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
        binormal = Converter::ToXMFloat3(CrossProduct(t, n).Unit());
    }

    void TextureVertex::CalculateBinormal()
    {
        Vector3 n  = Converter::ToVector3(normal);
        Vector3 t  = Converter::ToVector3(tangent);
        Vector3 b  = CrossProduct(t, n).Unit();
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
}
