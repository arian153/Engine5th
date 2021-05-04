#pragma once

#include <d3d11.h>
#include <directxmath.h>

namespace Engine5
{
    struct MatrixBufferType
    {
        DirectX::XMMATRIX mvp;
        DirectX::XMMATRIX world;
    };

    class MatrixBufferDX11
    {
    public:
        MatrixBufferDX11();
        ~MatrixBufferDX11();

    private:
    };
}
