#include "TextureVertex.hpp"
#include "../../Math/Utility/Converter.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace Engine5
{
    TextureVertex::TextureVertex()
        : position(), uv(), normal(), tangent(), binormal()
    {
    }

    TextureVertex::TextureVertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv)
        : position(p), uv(uv), normal(n), tangent(), binormal()
    {
    }

    TextureVertex::TextureVertex(Real px, Real py, Real pz, Real nx, Real ny, Real nz, Real u, Real v)
        : position(px, py, pz), uv(u, v), normal(nx, ny, nz), tangent(), binormal()
    {
    }

    TextureVertex::TextureVertex(Real px, Real py, Real pz, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz, Real u, Real v)
        : position(px, py, pz), uv(u, v), normal(nx, ny, nz), tangent(tx, ty, tz), binormal()
    {
        binormal = Converter::ToXMFloat3(Vector3(tx, ty, tz).CrossProduct(Vector3(nx, ny, nz)));
    }

    TextureVertex::TextureVertex(const Vector3& p, const Vector3& n, const Vector2& uv, const Vector3& t)
    {
        position = Converter::ToXMFloat3(p);
        this->uv = Converter::ToXMFloat2(uv);
        normal   = Converter::ToXMFloat3(n);
        tangent  = Converter::ToXMFloat3(t);
        binormal = Converter::ToXMFloat3(t.CrossProduct(n));
    }
}
