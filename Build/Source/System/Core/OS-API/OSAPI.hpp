#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Application;

    class OSAPI
    {
    public:
        explicit OSAPI(Application* application);
        virtual ~OSAPI();

        virtual void Initialize() = 0;
        virtual void Update(Real dt) = 0;
        virtual void Shutdown() = 0;

        virtual void SetConfineCursor(bool flag) = 0;
        virtual void SetShowCursor(bool flag) = 0;
        virtual void SetFullscreen(bool flag) = 0;
        virtual void SetClientResolution(int width, int height) = 0;
        virtual void SetMonitorResolution() = 0;

        virtual int  ClientWidth() const final;
        virtual int  ClientHeight() const final;
        virtual bool IsFullscreen() const final;
        virtual bool IsConfineCursor() const final;
        virtual bool IsShowCursor() const final;
        virtual bool IsInit() const final;
        virtual bool IsQuit() const final;
        virtual Real AspectRatio() const final;

        virtual Application* GetApplication() const final;

    protected:
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
