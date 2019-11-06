#pragma once
#include "../OSAPI.hpp"
#include <Windows.h>

namespace Engine5
{
    class WindowsAPI final : public OSAPI
    {
    public:
        explicit WindowsAPI(Application* application);
        ~WindowsAPI();

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        void SetConfineCursor(bool flag) override;
        void SetShowCursor(bool flag) override;
        void SetFullscreen(bool flag) override;
        void SetClientResolution(int width, int height) override;
        void SetMonitorResolution() override;
       
        LRESULT   MessageProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        void      MessagePump() const;
        void      AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start);
        DWORD     GetWindowModeRelatedResolution() const;
        HINSTANCE AppInstance() const;
        HWND      AppHWnd() const;

    private:
        friend class Application;

    private:
        //window data
        HINSTANCE m_h_instance;
        HWND      m_h_wnd = nullptr;
        DWORD     m_style = WINDOWED_STYLE_COMMON;

    private:
        const DWORD FULLSCREEN_STYLE      = WS_POPUP | WS_VISIBLE;
        const DWORD WINDOWED_STYLE        = WS_POPUP | WS_CAPTION;
        const DWORD WINDOWED_STYLE_COMMON = WS_OVERLAPPEDWINDOW;
    };
}
