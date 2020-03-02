#pragma once
#include <directxmath.h>
#include "../../../Math/Utility/MathDef.hpp"
#include "../../Renderer/APIMacro.hpp"
#include "../../Renderer/DX11/Vertex/TextureVertexDX11.hpp"

namespace Engine5
{
    class Vector2;
    class Vector3;

    class TextureVertex : public TextureVertexAPI
    {
    public:
        TextureVertex();
        TextureVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz);
        TextureVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz);
        TextureVertex(const Vector3& p, const Vector2& uv, const Vector3& n);
        TextureVertex(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t);

        void CalculateBinormal();
        void CalculateTangentAndBinormal();
    public:
    };
}
