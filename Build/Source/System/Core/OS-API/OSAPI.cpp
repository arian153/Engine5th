#include "OSAPI.hpp"
#include "Application.hpp"

namespace Engine5
{
    OSAPI::OSAPI(Application* application)
        : m_application(application)
    {
    }

    OSAPI::~OSAPI()
    {
    }

    int OSAPI::ClientWidth() const
    {
        return m_curr_client_width;
    }

    int OSAPI::ClientHeight() const
    {
        return m_curr_client_height;
    }

    bool OSAPI::IsFullscreen() const
    {
        return m_b_fullscreen;
    }

    bool OSAPI::IsConfineCursor() const
    {
        return m_b_confine_cursor;
    }

    bool OSAPI::IsShowCursor() const
    {
        return m_b_show_mouse_cursor;
    }

    bool OSAPI::IsInit() const
    {
        return m_b_init;
    }

    bool OSAPI::IsQuit() const
    {
        return m_b_quit;
    }

    Real OSAPI::AspectRatio() const
    {
        return static_cast<Real>(m_curr_client_width) / m_curr_client_height;
    }

    Application* OSAPI::GetApplication() const
    {
        return m_application;
    }
}
