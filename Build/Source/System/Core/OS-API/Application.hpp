#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class RenderSystem;
    class PhysicsSystem;
    class OSCommon;
    class TimeUtility;

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
        OSCommon*           GetOSAPI() const;

    private:
        void CalculateFrameStatus();
        void OnResize(int client_width, int client_height) const;
        void OnFullscreen();
        void OnQuit();

    private:
        friend class OSCommon;

    private:
        //time-frame data
        int  m_frame_count  = 0;
        Real m_time_elapsed = 0.0f;
        Real m_curr_fps     = 0.0f;
        Real m_curr_mspf    = 0.0f;

        //application name
        std::string m_application_caption = "Engine 5th";

    private:
        static inline Application* m_s_application = nullptr;
        //sub systems
        OSCommon*      m_os_api         = nullptr;
        RenderSystem*  m_render_system  = nullptr;
        PhysicsSystem* m_physics_system = nullptr;
        TimeUtility*   m_time_utility   = nullptr;
    };
}
