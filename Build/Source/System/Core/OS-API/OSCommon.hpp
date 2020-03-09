#pragma once
#include "OSAPI.hpp"
#include IncludeOSAPI

namespace Engine5
{
    class OSCommon : public OSAPI
    {
    public:
        explicit OSCommon(Application* application);
        ~OSCommon();

        void Initialize();
        void Shutdown();

        void SetConfineCursor(bool b_confine_cursor);
        void SetShowCursor(bool b_show_cursor);
        void SetFullscreen(bool b_fullscreen);
        void SetClientResolution(int width, int height);
        void SetMonitorResolution();
        void DispatchMessagePump() const;

        int  ClientWidth() const;
        int  ClientHeight() const;
        bool IsFullscreen() const;
        bool IsConfineCursor() const;
        bool IsShowCursor() const;
        bool IsInit() const;
        bool IsQuit() const;
        bool IsPaused() const;
        Real AspectRatio() const;
        Real MonitorScaleFactor() const;

    private :
        void OnResize() const;

    private:
        friend class OSAPI;

    private:
        bool m_b_loaded     = false;
        bool m_b_init       = false;

        //mouse option
        bool m_b_show_mouse_cursor = true;
        bool m_b_confine_cursor    = false;

        //window style option
        bool m_b_fullscreen = false;
        bool m_b_invisible = false;
        bool m_b_borderless = false;
        bool m_b_resizable = false;

        //window application state
        bool m_b_app_paused = false;
        bool m_b_minimized = false;
        bool m_b_maximized = false;
        bool m_b_quit = false;

        //application window size
        int m_curr_client_width = 1280;
        int m_curr_client_height = 720;
        int m_prev_client_width = 1280;
        int m_prev_client_height = 720;

        //actual monitor resolution
        int m_monitor_screen_width = 0;
        int m_monitor_screen_height = 0;
    };
}
