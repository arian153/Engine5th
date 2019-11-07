#include "Windows.hpp"
#include "../Application.hpp"
#include "../../Utility/CoreUtility.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
    // before CreateWindow returns, and thus before mhMainWnd is valid.
    auto os_api = Engine5::Application::GetApplication()->GetOSAPI();
    return static_cast<Engine5::WindowsAPI*>(os_api)->MessageProc(hwnd, msg, wparam, lparam);
}


namespace Engine5
{
    WindowsAPI::WindowsAPI(Application* application)
        : m_application(application)
    {
        m_h_instance = GetModuleHandle(nullptr);
        m_style      = WINDOWED_STYLE_COMMON;
    }

    WindowsAPI::~WindowsAPI()
    {
    }

    void WindowsAPI::Initialize()
    {
        SetMonitorResolution();
        WNDCLASS wc;
        //DEVMODE dm_screen_settings;
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc   = WindowProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = m_h_instance;
        wc.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName  = nullptr;
        wc.lpszClassName = L"E5hWnd";
        if (!RegisterClass(&wc))
        {
            MessageBox(nullptr, L"Register Class Failed.", nullptr, 0);
            m_b_init = false;
            return;
        }
        m_style = GetWindowModeRelatedResolution();

        // Compute window rectangle dimensions based on requested client area dimensions.
        RECT R = {0, 0, m_curr_client_width, m_curr_client_height};
        int  width_start, height_start;
        AdjustAndCenterWindow(m_style, R, width_start, height_start);
        int width  = R.right - R.left;
        int height = R.bottom - R.top;
        m_h_wnd    = CreateWindow(
                                  L"E5hWnd", StringToWString(m_application->m_application_caption).c_str(), m_style,
                                  width_start, height_start, width, height, nullptr, nullptr, m_h_instance, this
                                 );
        if (!m_h_wnd)
        {
            MessageBox(nullptr, L"Create Window Failed.", nullptr, 0);
            m_b_init = false;
            return;
        }
        if (m_b_fullscreen == true)
            SetFullscreen(true);
        ShowWindow(m_h_wnd, SW_SHOW);
        UpdateWindow(m_h_wnd);
        m_b_init = true;
    }

    void WindowsAPI::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        MessagePump();
    }

    void WindowsAPI::Shutdown()
    {
    }

    void WindowsAPI::SetConfineCursor(bool flag)
    {
        m_b_confine_cursor = flag;
        if (flag == true)
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

    void WindowsAPI::SetShowCursor(bool flag)
    {
        m_b_show_mouse_cursor = flag;
        ShowCursor(flag);
    }

    void WindowsAPI::SetFullscreen(bool flag)
    {
        if (flag == true)
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

        /*Create variables to adjust window size and start position*/
        RECT rect      = {0, 0, m_curr_client_width, m_curr_client_height};
        int  xStart    = 0, yStart = 0;
        m_b_fullscreen = flag;

        /*Check if we are going into full screen or not*/
        if (flag == true)
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
                m_b_fullscreen = false;
                m_style        = GetWindowModeRelatedResolution();
                ChangeDisplaySettings(nullptr, 0);/*If So Switch Back To The Desktop*/
                //("FullScreen is not supported. You are being switched to Windowed Mode");
                SetClientResolution(m_prev_client_width, m_prev_client_height);
            }
        }
        else
        {
            ChangeDisplaySettings(nullptr, 0);/*If So Switch Back To The Desktop*/
            m_style = GetWindowModeRelatedResolution();
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

    void WindowsAPI::SetClientResolution(int width, int height)
    {
        if (m_b_fullscreen == true)
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
        SetFullscreen(m_b_fullscreen);
    }

    void WindowsAPI::SetMonitorResolution()
    {
        m_monitor_screen_width  = GetSystemMetrics(SM_CXSCREEN);
        m_monitor_screen_height = GetSystemMetrics(SM_CYSCREEN);
    }


    LRESULT WindowsAPI::MessageProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
            // WM_ACTIVATE is sent when the window is activated or deactivated.  
            // We pause the game when the window is deactivated and unpause it 
            // when it becomes active.  
        case WM_ACTIVATE:
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                m_b_app_paused = true;
                //m_timer->Stop();
            }
            else
            {
                m_b_app_paused = false;
                //m_timer->Start();
            }
            break;

            // WM_SIZE is sent when the user resizes the window.  
        case WM_SIZE:
            // Save the new client area dimensions.
            if (m_b_fullscreen == false)
            {
                m_prev_client_width  = m_curr_client_width;
                m_prev_client_height = m_curr_client_height;
                m_curr_client_width  = LOWORD(lparam);
                m_curr_client_height = HIWORD(lparam);
            }
            if (wparam == SIZE_MINIMIZED)
            {
                m_b_app_paused = true;
                m_b_minimized  = true;
                m_b_maximized  = false;
            }
            else if (wparam == SIZE_MAXIMIZED)
            {
                m_b_app_paused = false;
                m_b_minimized  = false;
                m_b_maximized  = true;
                m_application->OnResize(m_curr_client_width, m_curr_client_height);
            }
            else if (wparam == SIZE_RESTORED)
            {
                // Restoring from minimized state?
                if (m_b_minimized)
                {
                    m_b_app_paused = false;
                    m_b_minimized  = false;
                    m_application->OnResize(m_curr_client_width, m_curr_client_height);
                }

                    // Restoring from maximized state?
                else if (m_b_maximized)
                {
                    m_b_app_paused = false;
                    m_b_maximized  = false;
                    m_application->OnResize(m_curr_client_width, m_curr_client_height);
                }
                else if (m_b_resizable)
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
                    m_application->OnResize(m_curr_client_width, m_curr_client_height);
                }
            }
            break;

            // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
        case WM_ENTERSIZEMOVE:
            m_b_app_paused = true;
            m_b_resizable = true;
            //m_timer->Stop();
            break;

            // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
            // Here we reset everything based on the new window dimensions.
        case WM_EXITSIZEMOVE:
            m_b_app_paused = false;
            m_b_resizable = false;
            //m_timer->Start();
            m_application->OnResize(m_curr_client_width, m_curr_client_height);
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
            //m_mouse_input->ProcMouseEvent(false, KeyID_Mouse::Mouse_Left, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_LBUTTONDOWN:
            //m_mouse_input->ProcMouseEvent(true, KeyID_Mouse::Mouse_Left, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_RBUTTONUP:
            //m_mouse_input->ProcMouseEvent(false, KeyID_Mouse::Mouse_Right, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_RBUTTONDOWN:
            //m_mouse_input->ProcMouseEvent(true, KeyID_Mouse::Mouse_Right, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_MBUTTONUP:
            //m_mouse_input->ProcMouseEvent(false, KeyID_Mouse::Mouse_Middle, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_MBUTTONDOWN:
            //m_mouse_input->ProcMouseEvent(true, KeyID_Mouse::Mouse_Middle, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_XBUTTONUP:
            //m_mouse_input->ProcMouseEvent(false, KeyID_Mouse::Mouse_X1, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_XBUTTONDOWN:
            //m_mouse_input->ProcMouseEvent(true, KeyID_Mouse::Mouse_X1, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_h_wnd);
            break;
        case WM_MOUSEMOVE:
            //m_mouse_input->ProcMouseEvent(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        case WM_MOUSEWHEEL:
            //m_mouse_input->ProcMouseWheel(GET_WHEEL_DELTA_WPARAM(wparam));
            break;
        case WM_CHAR:
            //m_keyboard_input->ProcessString(wparam);
            break;
        case WM_KEYDOWN:
            //m_keyboard_input->ProcKeyBoardEvent(true, wparam, lparam);
            break;
        case WM_KEYUP:
            //m_keyboard_input->ProcKeyBoardEvent(false, wparam, lparam);
            break;
         
            // WM_DESTROY is sent when the window is being destroyed.
        case WM_DESTROY:
            m_b_quit = true;
            //m_level_manager->SetQuit();
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

    void WindowsAPI::MessagePump() const
    {
        MSG msg = {nullptr};
        while (PeekMessage(&msg, m_h_wnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void WindowsAPI::AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start)
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

        /* Get start position for center */
        x_start = (dm.dmPelsWidth / 2) - (win_width / 2);
        y_start = (dm.dmPelsHeight / 2) - (win_height / 2);
    }

    DWORD WindowsAPI::GetWindowModeRelatedResolution() const
    {
        if (m_curr_client_height >= m_monitor_screen_height)
        {
            return WINDOWED_STYLE;
        }
        return WINDOWED_STYLE_COMMON;
    }

    HINSTANCE WindowsAPI::AppInstance() const
    {
        return m_h_instance;
    }

    HWND WindowsAPI::AppHWnd() const
    {
        return m_h_wnd;
    }

    int WindowsAPI::ClientWidth() const
    {
        return m_curr_client_width;
    }

    int WindowsAPI::ClientHeight() const
    {
        return m_curr_client_height;
    }

    bool WindowsAPI::IsFullscreen() const
    {
        return m_b_fullscreen;
    }

    bool WindowsAPI::IsConfineCursor() const
    {
        return m_b_confine_cursor;
    }

    bool WindowsAPI::IsShowCursor() const
    {
        return m_b_show_mouse_cursor;
    }

    bool WindowsAPI::IsInit() const
    {
        return m_b_init;
    }

    bool WindowsAPI::IsQuit() const
    {
        return m_b_quit;
    }

    Real WindowsAPI::AspectRatio() const
    {
        return static_cast<Real>(m_curr_client_width) / m_curr_client_height;
    }

    Application* WindowsAPI::GetApplication() const
    {
        return m_application;
    }
}
