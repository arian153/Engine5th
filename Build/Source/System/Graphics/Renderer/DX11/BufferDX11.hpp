#pragma once
#include <d3d11.h>

namespace Engine5
{
    class BufferDX11
    {
    public:
        BufferDX11();
        ~BufferDX11();

    public:
        ID3D11Buffer*        m_vertex_buffer  = nullptr;
        ID3D11Buffer*        m_index_buffer   = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
    };
}
