#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../../Graphics/RenderSystem.hpp"


namespace Engine5
{
    class WindowsAPI;

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
        WindowsAPI*         GetOSAPI() const;

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
        String m_application_caption = "Engine 5";

    private:
        static inline Application* m_s_application = nullptr;
        //sub systems
        WindowsAPI*   m_os_api        = nullptr;
        RenderSystem* m_render_system = nullptr;
    };
}
