#include "Application.hpp"

#include "Windows/Windows.hpp"

//systems
#include "../../Graphics/RenderSystem.hpp"
#include "../../Physics/PhysicsSystem.hpp"

//utilities
#include "../Utility/TimeUtility.hpp"
#include <string>

namespace Engine5
{
    Application::Application()
    {
        m_s_application = this;
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        Initialize();
        Update();
        Shutdown();
    }

    void Application::Initialize()
    {
        m_os_api = new WindowsAPI(this);
        m_os_api->Initialize();
        m_render_system = new RenderSystem(m_os_api);
        m_render_system->Initialize(1280, 720);
        m_physics_system = new PhysicsSystem();
        m_physics_system->Initialize();
        m_time_utility = new TimeUtility();
        m_time_utility->Reset();
    }

    void Application::Update()
    {
        while (m_os_api->IsQuit() == false && m_os_api->IsInit() == true)
        {
            CalculateFrameStatus();
            m_os_api->Update();
        }
    }

    void Application::Shutdown()
    {
        if (m_time_utility != nullptr)
        {
            delete m_time_utility;
            m_time_utility = nullptr;
        }
        if (m_physics_system != nullptr)
        {
            m_physics_system->Shutdown();
            delete m_physics_system;
            m_physics_system = nullptr;
        }
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

    void Application::CalculateFrameStatus()
    {
        m_frame_count++;
        // Compute averages over one second period.
        if (m_time_utility->TotalTime() - m_time_elapsed >= 1.0f)
        {
            m_curr_fps  = (float)m_frame_count; // fps = frameCnt / 1
            m_curr_mspf = 1000.0f / m_curr_fps;
            // Reset for next average.
            m_frame_count = 0;
            m_time_elapsed += 1.0f;
        }
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
