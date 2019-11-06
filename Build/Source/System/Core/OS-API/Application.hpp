#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "OSAPI.hpp"
#include "../../Graphics/RenderSystem.hpp"


namespace Engine5
{
    class JobSystem;
    class WindowsAPI;

    enum class OSAPIMode
    {
        Windows
    };

    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

        void Initialize();
        void Update();
        void Shutdown();

        Real CurrentFPS() const;
        Real CurrentMSPF() const;
        Real CurrentSPF() const;

        static Application* GetApplication();
        OSAPI*              GetOSAPI() const;

    private:
        void CalculateFrameStats();
        void OnResize(int client_width, int client_height) const;
        void OnFullscreen();
        void OnQuit();

    private:
        friend class WindowsAPI;


    private:
        //time-frame data
        int  m_frame_count  = 0;
        Real m_time_elapsed = 0.0f;
        Real m_curr_fps     = 0.0f;
        Real m_curr_mspf    = 0.0f;

        //application name
        std::string      m_application_caption = "Engine 5";
        OSAPIMode        m_os_api_mode;
        RenderingAPIMode m_rendering_api_mode;

    private:
        static inline Application* m_s_application = nullptr;
        //sub systems
        WindowsAPI*        m_os_api        = nullptr;
        RenderSystem* m_render_system = nullptr;
    };
}
