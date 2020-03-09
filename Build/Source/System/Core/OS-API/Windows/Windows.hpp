#pragma once
#include <Windows.h>
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Application;

    class OSWin32
    {
    public:
        explicit OSWin32(Application* application);
        ~OSWin32();

        void Initialize();
        void Update() const;
        void Shutdown();

        void SetConfineCursor(bool flag);
        void SetShowCursor(bool flag);
        void SetFullscreen(bool flag);
        void SetClientResolution(int width, int height);
        void SetMonitorResolution();

        LRESULT   MessageProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        void      MessagePump() const;
        void      AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start) const;
        DWORD     GetWindowModeRelatedResolution() const;
        HINSTANCE AppInstance() const;
        HWND      AppHWnd() const;
        Real      MonitorScaleFactor() const;

        int  ClientWidth() const;
        int  ClientHeight() const;
        bool IsFullscreen() const;
        bool IsConfineCursor() const;
        bool IsShowCursor() const;
        bool IsInit() const;
        bool IsQuit() const;
        Real AspectRatio() const;

        Application* GetApplication() const;

    private:
        friend class Application;

    private:
        const DWORD FULLSCREEN_STYLE      = WS_POPUP | WS_VISIBLE;
        const DWORD WINDOWED_STYLE        = WS_POPUP | WS_CAPTION;
        const DWORD WINDOWED_STYLE_COMMON = WS_OVERLAPPEDWINDOW;

    private:
        //window data
        HINSTANCE m_h_instance;
        HWND      m_h_wnd = nullptr;
        DWORD     m_style = WINDOWED_STYLE_COMMON;

        //window style option
        bool m_b_fullscreen = false;
        bool m_b_invisible  = false;
        bool m_b_borderless = false;
        bool m_b_resizable  = false;

        //application state
        bool m_b_loaded     = false;
        bool m_b_init       = false;
        bool m_b_quit       = false;
        bool m_b_app_paused = false;
        bool m_b_minimized  = false;
        bool m_b_maximized  = false;

        //mouse option
        bool m_b_show_mouse_cursor = true;
        bool m_b_confine_cursor    = false;

        //application window size
        int m_curr_client_width  = 1280;
        int m_curr_client_height = 720;
        int m_prev_client_width  = 1280;
        int m_prev_client_height = 720;

        //actual monitor resolution
        int m_monitor_screen_width  = 0;
        int m_monitor_screen_height = 0;

        Application* m_application = nullptr;
    };
}
