#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include "SubsystemFlag.hpp"

namespace Engine5
{
    class ComponentRegistry;
    class ObjectFactory;
    class RenderSystem;
    class PhysicsSystem;
    class ObjectManager;
    class ComponentManager;
    class Scene;
    class World;
    class SpaceManager;

    class Space
    {
    public:
        Space();
        ~Space();

        void Initialize(eSubsystemFlag flag, PhysicsSystem* physics_system, RenderSystem* render_system, ObjectFactory* obj_factory, ComponentRegistry* cmp_registry);
        void Update(Real dt) const;
        void Shutdown(PhysicsSystem* physics_system, RenderSystem* render_system);

        void ConnectSubsystem(ComponentManager* component_manager);
        void ConnectSubsystem(ObjectManager* object_manager);
        void ConnectSubsystem(Scene* scene);
        void ConnectSubsystem(World* world);

        ObjectManager*    GetObjectManager() const;
        ComponentManager* GetComponentManager() const;
        Scene*            GetScene() const;
        World*            GetWorld() const;

    private:

    private:
        friend class SpaceManager;

    private:
        bool              m_b_activate        = false;
        eSubsystemFlag    m_subsystem_flag    = eSubsystemFlag::None;
        SpaceManager*     m_space_manager     = nullptr;
        ComponentManager* m_component_manager = nullptr;
        ObjectManager*    m_object_manager    = nullptr;
        Scene*            m_scene             = nullptr;
        World*            m_world             = nullptr;
    };
}
