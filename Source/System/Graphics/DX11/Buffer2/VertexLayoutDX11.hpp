#pragma once
#include <d3d11.h>

namespace Engine5
{
    class VertexLayoutDX11
    {
    public:
        VertexLayoutDX11();
        ~VertexLayoutDX11();

    protected:
        ID3D11InputLayout*   m_layout         = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
    };
}
