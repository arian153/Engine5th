#include "Application.hpp"

#include "Windows/Windows.hpp"

namespace Engine5
{
    Application::Application()
    {
        m_s_application      = this;
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        Initialize();

        //m_job_system->Run(MainCallback);
        Update();
        Shutdown();
    }

    void Application::Initialize()
    {
        m_os_api = new WindowsAPI(this);
        m_os_api->Initialize();
        m_render_system = new RenderSystem(m_os_api);
        m_render_system->Initialize(1280, 720);
    }

    void Application::Update()
    {
        while (m_os_api->IsQuit() == false && m_os_api->IsInit() == true)
        {
            m_os_api->Update(0.16f);
            m_render_system->Update(0.16f);
            m_curr_fps = 0.16f;
        }
    }

    void Application::Shutdown()
    {
        if (m_render_system != nullptr)
        {
            m_render_system->Shutdown();
            delete m_render_system;
            m_render_system = nullptr;
        }
        if (m_os_api != nullptr)
        {
            m_os_api->Shutdown();
            delete m_os_api;
            m_os_api = nullptr;
        }
    }

    Real Application::CurrentFPS() const
    {
        return m_curr_fps;
    }

    Real Application::CurrentMSPF() const
    {
        return m_curr_mspf;
    }

    Real Application::CurrentSPF() const
    {
        return (1.0f / m_curr_fps);
    }

    Application* Application::GetApplication()
    {
        return m_s_application;
    }

    WindowsAPI* Application::GetOSAPI() const
    {
        return m_os_api;
    }


    void Application::CalculateFrameStats()
    {
    }

    void Application::OnResize(int client_width, int client_height) const
    {
        if (m_render_system != nullptr)
        {
            m_render_system->OnResize(client_width, client_height);
        }
    }

    void Application::OnFullscreen()
    {
    }

    void Application::OnQuit()
    {
    }
}
