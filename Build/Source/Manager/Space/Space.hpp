#pragma once
#include "../../System/Math/Utility/MathDef.hpp"

namespace Engine5
{
    class ComponentFactory;
    class ObjectFactory;
    class RenderSystem;
    class PhysicsSystem;
    class ObjectManager;
    class ComponentManager;
    class Scene;
    class World;

    class Space
    {
    public:
        Space();
        ~Space();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:
        bool m_b_activate = false;

        void InitializeWorld(PhysicsSystem* physics_system);
        void InitializeScene(RenderSystem* render_system);
        void InitializeManager(ObjectFactory* obj_factory, ComponentFactory* cmp_factory);


        ComponentManager* m_component_manager = nullptr;
        ObjectManager*    m_object_manager    = nullptr;
        Scene*            m_scene             = nullptr;
        World*            m_world             = nullptr;
    };
}
