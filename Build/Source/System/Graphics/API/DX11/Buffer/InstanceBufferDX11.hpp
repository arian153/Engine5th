#pragma once
#include <d3d11.h>

namespace Engine5
{
    class InstanceBufferDX11
    {
    public:
        InstanceBufferDX11();
        ~InstanceBufferDX11();

    protected:
        ID3D11Buffer*        m_vertex_buffer   = nullptr;
        ID3D11Buffer*        m_index_buffer    = nullptr;
        ID3D11Buffer*        m_instance_buffer = nullptr;
        ID3D11DeviceContext* m_device_context  = nullptr;
    };
}
