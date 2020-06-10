#pragma once
#include "SubsystemFlag.hpp"
#include <vector>

namespace Engine5
{
    class LogicSystem;
    class ResourceManager;
    class JsonResource;
    class Space;
    class ObjectFactory;
    class ComponentRegistry;
    class PhysicsSystem;
    class RenderSystem;

    class SpaceManager
    {
    public:
        SpaceManager() = delete;
        explicit SpaceManager(
            PhysicsSystem* physics_system, 
            RenderSystem* render_system, 
            ObjectFactory* object_factory, 
            ComponentRegistry* component_registry, 
            ResourceManager* resource_manager, 
            LogicSystem* logic_system);
        ~SpaceManager();

        void Initialize();
        void Shutdown();

        Space* GetGlobalSpace() const;
        Space* CreateSpace(eSubsystemFlag flag);
        Space* CreateSpace(JsonResource* resource);
        void   RemoveSpace(Space* space);

    private:
        friend class Space;

    private:
        eSubsystemFlag m_global_flag = eSubsystemFlag::None;

        Space*              m_global_space = nullptr;
        std::vector<Space*> m_spaces;
        std::vector<Space*> m_active_spaces;

        PhysicsSystem*   m_physics_system   = nullptr;
        RenderSystem*    m_render_system    = nullptr;
        LogicSystem*     m_logic_system     = nullptr;
        ResourceManager* m_resource_manager = nullptr;

        //factory
        ObjectFactory*     m_object_factory     = nullptr;
        ComponentRegistry* m_component_registry = nullptr;
    };
}
