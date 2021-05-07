#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct CameraBufferType
    {
        DirectX::XMFLOAT3 camera_position;
        Real              padding;
    };

    class CameraBufferDX11
    {
    public:
        CameraBufferDX11();
        ~CameraBufferDX11();

    protected:
        ID3D11Buffer*        m_camera_buffer  = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
    };
}
