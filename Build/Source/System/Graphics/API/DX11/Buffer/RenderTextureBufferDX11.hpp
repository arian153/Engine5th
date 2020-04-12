#pragma once
#include <d3d11.h>
#include "../../../../Core/Utility/CoreDef.hpp"
#include "../../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class RenderTextureBufferDX11
    {
    public:
        RenderTextureBufferDX11();
        ~RenderTextureBufferDX11();

        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);
        bool BuildBuffer(Real left, Real right, Real top, Real bottom);

    protected:
        ID3D11Buffer*        m_vertex_buffer  = nullptr;
        ID3D11Buffer*        m_index_buffer   = nullptr;
        ID3D11Device*        m_device         = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;

        U32 m_vertex_count = 0;
        U32 m_index_count  = 0;
    };
}
