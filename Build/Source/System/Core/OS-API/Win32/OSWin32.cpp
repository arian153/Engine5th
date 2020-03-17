#pragma warning( disable : 26812)

#include "OSWin32.hpp"
#include "../Application.hpp"
#include "../../Utility/CoreUtility.hpp"
#include <ShellScalingApi.h>
#include "../OSCommon.hpp"
#include "../../Utility/TimeUtility.hpp"
#include "../../../../Manager/Level/LevelManager.hpp"
#include "../../Input/InputCommon.hpp"
#include "../../Input/MouseInput.hpp"
#include "../../Input/KeyboardInput.hpp"
#include <WindowsX.h>

namespace
{
    Engine5::OSWin32* g_win32 = nullptr;

    LRESULT CALLBACK ProcessWindow(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        return g_win32->ProcessMessage(hwnd, msg, wparam, lparam);
    }
}

namespace Engine5
{
    OSWin32::OSWin32(Application* application, OSCommon* os_common)
        : m_application(application), m_os_common(os_common)
    {
        m_h_instance = GetModuleHandle(nullptr);
        m_style      = WINDOWED_STYLE_COMMON;
        g_win32      = this;
    }

    OSWin32::~OSWin32()
    {
    }

    LRESULT OSWin32::ProcessMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) const
    {
        switch (msg)
        {
            // WM_ACTIVATE is sent when the window is activated or deactivated.  
            // We pause the game when the window is deactivated and unpause it 
            // when it becomes active.  
        case WM_ACTIVATE:
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                m_os_common->m_b_app_paused = true;
                m_os_common->m_application_timer->Stop();
            }
            else
            {
                m_os_common->m_b_app_paused = false;
                m_os_common->m_application_timer->Start();
            }
            break;
            // WM_SIZE is sent when the user resizes the window.  
        case WM_SIZE:
            // Save the new client area dimensions.
            if (m_os_common->m_window_mode == eWindowMode::Fullscreen)
            {
                m_os_common->m_prev_client_width  = m_os_common->m_curr_client_width;
                m_os_common->m_prev_client_height = m_os_common->m_curr_client_height;
                m_os_common->m_curr_client_width  = LOWORD(lparam);
                m_os_common->m_curr_client_height = HIWORD(lparam);
            }
            if (wparam == SIZE_MINIMIZED)
            {
                m_os_common->m_b_app_paused = true;
                m_os_common->m_b_minimized  = true;
                m_os_common->m_b_maximized  = false;
            }
            else if (wparam == SIZE_MAXIMIZED)
            {
                m_os_common->m_b_app_paused = false;
                m_os_common->m_b_minimized  = false;
                m_os_common->m_b_maximized  = true;
                m_os_common->OnResize();
            }
            else if (wparam == SIZE_RESTORED)
            {
                // Restoring from minimized state?
                if (m_os_common->m_b_minimized)
                {
                    m_os_common->m_b_app_paused = false;
                    m_os_common->m_b_minimized  = false;
                    m_os_common->OnResize();
                }
                    // Restoring from maximized state?
                else if (m_os_common->m_b_maximized)
                {
                    m_os_common->m_b_app_paused = false;
                    m_os_common->m_b_maximized  = false;
                    m_os_common->OnResize();
                }
                else if (m_os_common->m_b_resizable)
                {
                    // If user is dragging the resize bars, we do not resize 
                    // the buffers here because as the user continuously 
                    // drags the resize bars, a stream of WM_SIZE messages are
                    // sent to the window, and it would be pointless (and slow)
                    // to resize for each WM_SIZE message received from dragging
                    // the resize bars.  So instead, we reset after the user is 
                    // done resizing the window and releases the resize bars, which 
                    // sends a WM_EXITSIZEMOVE message.
                }
                else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
                {
                    m_os_common->OnResize();
                }
            }
            break;
            // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
        case WM_ENTERSIZEMOVE:
            m_os_common->m_b_app_paused = true;
            m_os_common->m_b_resizable = true;
            m_os_common->m_application_timer->Stop();
            break;
            // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
            // Here we reset everything based on the new window dimensions.
        case WM_EXITSIZEMOVE:
            m_os_common->m_b_app_paused = false;
            m_os_common->m_b_resizable = false;
            m_os_common->m_application_timer->Start();
            m_os_common->OnResize();
            break;
            // The WM_MENUCHAR message is sent when a menu is active and the user presses 
            // a key that does not correspond to any mnemonic or accelerator key. 
        case WM_MENUCHAR:
            // Don't beep when we alt-enter.
            return MAKELRESULT(0, MNC_CLOSE);
            // Catch this message so to prevent the window from becoming too small.
        case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lparam)->ptMinTrackSize.x = 200;
            ((MINMAXINFO*)lparam)->ptMinTrackSize.y = 200;
            break;
        case WM_LBUTTONUP:
            m_mouse_input->ProcessMouseEvent(false, eKeyCodeMouse::Left, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_LBUTTONDOWN:
            m_mouse_input->ProcessMouseEvent(true, eKeyCodeMouse::Left, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_RBUTTONUP:
            m_mouse_input->ProcessMouseEvent(false, eKeyCodeMouse::Right, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_RBUTTONDOWN:
            m_mouse_input->ProcessMouseEvent(true, eKeyCodeMouse::Right, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_MBUTTONUP:
            m_mouse_input->ProcessMouseEvent(false, eKeyCodeMouse::Middle, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_MBUTTONDOWN:
            m_mouse_input->ProcessMouseEvent(true, eKeyCodeMouse::Middle, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_XBUTTONUP:
            m_mouse_input->ProcessMouseEvent(false, eKeyCodeMouse::X1, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_XBUTTONDOWN:
            m_mouse_input->ProcessMouseEvent(true, eKeyCodeMouse::X1, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_MOUSEMOVE:
            m_mouse_input->ProcessMouseEvent(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        case WM_MOUSEWHEEL:
            m_mouse_input->ProcessMouseWheel(GET_WHEEL_DELTA_WPARAM(wparam));
            break;
        case WM_CHAR:
            m_keyboard_input->ProcessString(wparam);
            break;
            // WM_DESTROY is sent when the window is being destroyed.
        case WM_DESTROY:
            m_os_common->m_level_manager->SetQuit();
            PostQuitMessage(0);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_SYSCOMMAND:
            if (wparam == SC_KEYMENU && (lparam >> 16) <= 0)
                break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    }

    void OSWin32::AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start) const
    {
        DEVMODE dm = {0};
        /*Get the size of the screen*/
        dm.dmSize = sizeof(dm);
        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
        /*Make client area of window the correct size */
        AdjustWindowRect(&size, style, false);
        /*Calculate new width and height */
        int win_width  = size.right - size.left;
        int win_height = size.bottom - size.top;
        //Real monitor_scale   = m_os_common->MonitorScaleFactor();
        Real scale_factor    = 2.0f * m_os_common->m_screen_scale;
        Real movement_width  = (Real)win_width / scale_factor;
        Real movement_height = (Real)win_height / scale_factor;
        Real width           = (Real)dm.dmPelsWidth / scale_factor;
        Real height          = (Real)dm.dmPelsHeight / scale_factor;
        /* Get start position for center */
        x_start = static_cast<int>(width - movement_width);
        y_start = static_cast<int>(height - movement_height);
    }

    DWORD OSWin32::GetWindowedStyle() const
    {
        return m_os_common->m_curr_client_height >= m_os_common->m_monitor_screen_height ? WINDOWED_STYLE : WINDOWED_STYLE_COMMON;
    }

    HINSTANCE OSWin32::AppInstance() const
    {
        return m_h_instance;
    }

    HWND OSWin32::AppHWnd() const
    {
        return m_h_wnd;
    }

    void OSWin32::SetInput(InputCommon* input)
    {
        m_keyboard_input = input->GetKeyboardInput();
        m_mouse_input    = input->GetMouseInput();
    }

    DWORD OSWin32::GetWindowModeStyle(eWindowMode window_mode) const
    {
        switch (window_mode)
        {
        case eWindowMode::Windowed:
            return GetWindowedStyle();
        case eWindowMode::Borderless:
            return BORDERLESS_STYLE;
        case eWindowMode::Fullscreen:
            return FULLSCREEN_STYLE;
        default:
            return GetWindowedStyle();
        }
    }

    OSCommon::OSCommon(Application* application)
        : OSWin32(application, this)
    {
        m_application_timer         = m_application->GetApplicationTimer();
        ApplicationSetting& setting = application->m_initial_setting;
        m_b_show_cursor             = setting.b_show_cursor;
        m_b_confine_cursor          = setting.b_confine_cursor;
        //window style option
        m_window_mode         = setting.window_mode;
        m_application_caption = StringToWString(setting.caption);
        //application window size
        m_curr_client_width  = setting.screen_width;
        m_curr_client_height = setting.screen_height;
        m_prev_client_width  = setting.screen_width;
        m_prev_client_height = setting.screen_height;
        m_screen_scale       = setting.screen_scale;
    }

    OSCommon::~OSCommon()
    {
    }

    void OSCommon::Initialize()
    {
        SetMonitorResolution();
        WNDCLASS window_class;
        //DEVMODE dm_screen_settings;
        window_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        window_class.lpfnWndProc   = ProcessWindow;
        window_class.cbClsExtra    = 0;
        window_class.cbWndExtra    = 0;
        window_class.hInstance     = m_h_instance;
        window_class.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
        window_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        window_class.hbrBackground = nullptr;//(HBRUSH)GetStockObject(BLACK_BRUSH);
        window_class.lpszMenuName  = nullptr;
        window_class.lpszClassName = L"E5hWnd";
        if (!RegisterClass(&window_class))
        {
            MessageBox(nullptr, L"Register Class Failed.", nullptr, 0);
            m_b_init = false;
            return;
        }
        m_style = GetWindowedStyle();
        // Compute window rectangle dimensions based on requested client area dimensions.
        RECT window_rect = {0, 0, m_curr_client_width, m_curr_client_height};
        int  width_start, height_start;
        AdjustAndCenterWindow(m_style, window_rect, width_start, height_start);
        int width  = window_rect.right - window_rect.left;
        int height = window_rect.bottom - window_rect.top;
        m_h_wnd    = CreateWindow(
                                  L"E5hWnd", m_os_common->m_application_caption.c_str(), m_style,
                                  width_start, height_start, width, height, nullptr, nullptr, m_h_instance, this
                                 );
        if (!m_h_wnd)
        {
            MessageBox(nullptr, L"Create Window Failed.", nullptr, 0);
            m_b_init = false;
            return;
        }
        if (m_window_mode != eWindowMode::Windowed)
        {
            SetWindowMode(m_window_mode);
        }
        ShowWindow(m_h_wnd, SW_SHOW);
        UpdateWindow(m_h_wnd);
        m_b_init = true;
    }

    void OSCommon::Shutdown()
    {
    }

    void OSCommon::SetConfineCursor(bool b_confine_cursor)
    {
        m_b_confine_cursor = b_confine_cursor;
        if (b_confine_cursor == true)
        {
            RECT Clip;
            GetClientRect(m_h_wnd, &Clip);
            ClientToScreen(m_h_wnd, (LPPOINT)&Clip);
            ClientToScreen(m_h_wnd, (LPPOINT)(&Clip.right));
            ClipCursor(&Clip);
        }
        else
        {
            ClipCursor(nullptr);
        }
    }

    void OSCommon::SetShowCursor(bool b_show_cursor)
    {
        m_b_show_cursor = b_show_cursor;
        ShowCursor(b_show_cursor);
    }

    void OSCommon::SetWindowMode(eWindowMode window_mode)
    {
        if (window_mode == eWindowMode::Fullscreen)
        {
            if (m_curr_client_width > m_monitor_screen_width)
            {
                m_curr_client_width = m_monitor_screen_width;
            }
            if (m_curr_client_height > m_monitor_screen_height)
            {
                m_curr_client_height = m_monitor_screen_height;
            }
        }
        eWindowMode prev_mode = m_window_mode;
        m_window_mode         = window_mode;
        /*Create variables to adjust window size and start position*/
        RECT rect   = {0, 0, m_curr_client_width, m_curr_client_height};
        int  xStart = 0, yStart = 0;
        /*Check if we are going into full screen or not*/
        if (window_mode == eWindowMode::Fullscreen)
        {
            /*Get the current display settings*/
            DEVMODE dmScreenSettings;
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dmScreenSettings);
            /*Change the resolution to the resolution of my window*/
            dmScreenSettings.dmPelsWidth  = (DWORD)m_curr_client_width;
            dmScreenSettings.dmPelsHeight = (DWORD)m_curr_client_height;
            //dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            /*Make sure my window style is full screen*/
            m_style = FULLSCREEN_STYLE;
            //ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
            /*Check if it worked.  If it didn't set to window mode.*/
            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                m_window_mode = prev_mode;
                m_style       = GetWindowModeStyle(prev_mode);
                ChangeDisplaySettings(nullptr, 0);
                //Switch Back To The Desktop
                //("FullScreen is not supported. You are being switched to Windowed Mode");
                SetClientResolution(m_prev_client_width, m_prev_client_height);
            }
        }
        else
        {
            ChangeDisplaySettings(nullptr, 0);/*If So Switch Back To The Desktop*/
            m_style = GetWindowModeStyle(window_mode);
        }
        /*This will change my windows style*/
        SetWindowLong(m_h_wnd, GWL_STYLE, m_style);
        /*This will make window the correct size and find the start position*/
        AdjustAndCenterWindow(m_style, rect, xStart, yStart);
        /*This changes my window size and start position*/
        MoveWindow(m_h_wnd, xStart, yStart, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        /*This is required after SetWindowLong*/
        ShowWindow(m_h_wnd, SW_SHOWNORMAL);
        /*This sets my window to the front.*/
        SetForegroundWindow(m_h_wnd);
    }

    void OSCommon::SetQuit(bool b_quit)
    {
        m_b_quit = b_quit;
    }

    void OSCommon::SetClientResolution(int width, int height)
    {
        if (m_window_mode == eWindowMode::Fullscreen)
        {
            if (width > m_monitor_screen_width)
                return;
            if (height > m_monitor_screen_height)
                return;
        }
        m_prev_client_width  = m_curr_client_width;
        m_prev_client_height = m_curr_client_height;
        m_curr_client_width  = width;
        m_curr_client_height = height;
        SetWindowMode(m_window_mode);
    }

    void OSCommon::SetMonitorResolution()
    {
        m_monitor_screen_width  = GetSystemMetrics(SM_CXSCREEN);
        m_monitor_screen_height = GetSystemMetrics(SM_CYSCREEN);
    }

    void OSCommon::DispatchMessagePump() const
    {
        m_b_app_paused ? DispatchPaused() : DispatchActive();
    }

    void OSCommon::DispatchPaused() const
    {
        MSG msg = {nullptr};
        while (GetMessage(&msg, m_h_wnd, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            //exit pause state
            if (m_b_app_paused == false)
            {
                break;
            }
        }
    }

    void OSCommon::DispatchActive() const
    {
        MSG msg = {nullptr};
        while (PeekMessage(&msg, m_h_wnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    int OSCommon::ClientWidth() const
    {
        return m_curr_client_width;
    }

    int OSCommon::ClientHeight() const
    {
        return m_curr_client_height;
    }

    bool OSCommon::IsFullscreen() const
    {
        return m_window_mode == eWindowMode::Fullscreen;
    }

    bool OSCommon::IsConfineCursor() const
    {
        return m_b_confine_cursor;
    }

    bool OSCommon::IsShowCursor() const
    {
        return m_b_confine_cursor;
    }

    bool OSCommon::IsInit() const
    {
        return m_b_init;
    }

    bool OSCommon::IsQuit() const
    {
        return m_b_quit;
    }

    bool OSCommon::IsPaused() const
    {
        return m_b_app_paused;
    }

    Real OSCommon::AspectRatio() const
    {
        return static_cast<Real>(m_curr_client_width) / m_curr_client_height;
    }

    Real OSCommon::MonitorScaleFactor() const
    {
        DEVICE_SCALE_FACTOR result       = GetScaleFactorForDevice(DEVICE_PRIMARY);
        Real                scale_result = -1.0f;
        switch (result)
        {
        case DEVICE_SCALE_FACTOR_INVALID:
            break;
        case SCALE_100_PERCENT:
            scale_result = 1.0f;
            break;
        case SCALE_120_PERCENT:
            scale_result = 1.2f;
            break;
        case SCALE_125_PERCENT:
            scale_result = 1.25f;
            break;
        case SCALE_140_PERCENT:
            scale_result = 1.4f;
            break;
        case SCALE_150_PERCENT:
            scale_result = 1.5f;
            break;
        case SCALE_160_PERCENT:
            scale_result = 1.6f;
            break;
        case SCALE_175_PERCENT:
            scale_result = 1.75f;
            break;
        case SCALE_180_PERCENT:
            scale_result = 1.8f;
            break;
        case SCALE_200_PERCENT:
            scale_result = 2.0f;
            break;
        case SCALE_225_PERCENT:
            scale_result = 2.25f;
            break;
        case SCALE_250_PERCENT:
            scale_result = 2.5f;
            break;
        case SCALE_300_PERCENT:
            scale_result = 3.0f;
            break;
        case SCALE_350_PERCENT:
            scale_result = 3.5f;
            break;
        case SCALE_400_PERCENT:
            scale_result = 4.0f;
            break;
        case SCALE_450_PERCENT:
            scale_result = 4.5f;
            break;
        case SCALE_500_PERCENT:
            scale_result = 5.0f;
            break;
        default:
            scale_result = 1.0f;
            break;
        }
        return scale_result;
    }

    void OSCommon::SetLevelManager(LevelManager* level_manager)
    {
        m_level_manager = level_manager;
    }

    void OSCommon::OnResize() const
    {
        m_application->OnResize(m_curr_client_width, m_curr_client_height);
    }
}
