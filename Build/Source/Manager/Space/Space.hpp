#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include "SpaceFlag.hpp"

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

        void Initialize();
        void Update(Real dt) const;
        void Shutdown();

    private:
        void InitializeWorld(PhysicsSystem* physics_system);
        void InitializeScene(RenderSystem* render_system);
        void InitializeManager(ObjectFactory* obj_factory);
        void InitializeManager(ComponentRegistry* cmp_registry);

        void ShutdownWorld(PhysicsSystem* physics_system);
        void ShutdownScene(RenderSystem* render_system);
        void ShutdownManager(ComponentManager* cmp_manager);
        void ShutdownManager(ObjectManager* obj_manager);

    private:
        friend class SpaceManager;

    private:
        bool              m_b_activate        = false;
        SpaceManager*     m_space_manager     = nullptr;
        ComponentManager* m_component_manager = nullptr;
        ObjectManager*    m_object_manager    = nullptr;
        Scene*            m_scene             = nullptr;
        World*            m_world             = nullptr;
    };
}
