#include "DeferredShaderDX11.hpp"

namespace Engine5
{
    DeferredShaderDX11::DeferredShaderDX11()
    {
    }

    DeferredShaderDX11::~DeferredShaderDX11()
    {
    }

    void DeferredShaderDX11::SetHWnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void DeferredShaderDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void DeferredShaderDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }
}
