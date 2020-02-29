#pragma once
#include <d3d11.h>

namespace Engine5
{
    class ShaderManagerDX11
    {
    public:
        ShaderManagerDX11();
        ~ShaderManagerDX11();

        void SetHWnd(HWND hwnd);
        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);

    protected:
        ID3D11DeviceContext* m_device_context = nullptr;
        ID3D11Device*        m_device         = nullptr;
        HWND                 m_hwnd           = nullptr;
    };
}
