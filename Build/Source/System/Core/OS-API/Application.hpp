#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class FileUtility;
    class ResourceManager;
    class InputCommon;
    class ObjectFactory;
    class ComponentRegistry;
    class RenderSystem;
    class PhysicsSystem;
    class OSCommon;
    class TimeUtility;
    class LevelManager;
    class SpaceManager;
    class FrameUtility;

    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void Initialize();
        void Update() const;
        void Shutdown();

        //subsystem getter
        Application*       GetApplication();
        OSCommon*          GetOperatingSystem() const;
        RenderSystem*      GetRenderSystem() const;
        PhysicsSystem*     GetPhysicsSystem() const;
        FrameUtility*      GetFrameUtility() const;
        TimeUtility*       GetApplicationTimer() const;
        LevelManager*      GetLevelManager() const;
        SpaceManager*      GetSpaceManager() const;
        ObjectFactory*     GetObjectFactory() const;
        ComponentRegistry* GetComponentRegistry() const;
        InputCommon*       GetInput() const;
        FileUtility*       GetFileUtility() const;
        ResourceManager*   GetResourceManager() const;

    private:
        void OnResize(int client_width, int client_height) const;
        void OnFullscreen();
        void OnQuit();

    private:
        friend class OSCommon;
        friend class LevelManager;

    private:
        //application name
        std::string m_application_caption = "Engine 5th";

    private:
        //sub systems
        OSCommon*          m_operating_system   = nullptr;
        RenderSystem*      m_render_system      = nullptr;
        PhysicsSystem*     m_physics_system     = nullptr;
        TimeUtility*       m_time_utility       = nullptr;
        FrameUtility*      m_frame_utility      = nullptr;
        LevelManager*      m_level_manager      = nullptr;
        SpaceManager*      m_space_manager      = nullptr;
        ObjectFactory*     m_object_factory     = nullptr;
        ComponentRegistry* m_component_registry = nullptr;
        InputCommon*       m_input              = nullptr;
        FileUtility*       m_file_utility       = nullptr;
        ResourceManager*   m_resource_manager   = nullptr;
    };
}
