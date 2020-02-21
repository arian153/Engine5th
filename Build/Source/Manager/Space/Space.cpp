#include "Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Physics/PhysicsSystem.hpp"
#include "../../System/Graphics/RenderSystem.hpp"
#include "SpaceManager.hpp"
#include "../Object/ObjectManager.hpp"
#include "../Component/ComponentManager.hpp"

namespace Engine5
{
    Space::Space()
    {
    }

    Space::~Space()
    {
    }

    void Space::Initialize(eSubsystemFlag flag)
    {
        if (HasFlag(flag, eSubsystemFlag::ComponentManager))
        {
        }
        if (HasFlag(flag, eSubsystemFlag::ObjectManager))
        {
        }
        if (HasFlag(flag, eSubsystemFlag::Scene))
        {
        }
        if (HasFlag(flag, eSubsystemFlag::World))
        {
            //InitializeWorld();
        }
    }

    void Space::Update(Real dt)
    {
        //update world
        //update scene
    }

    void Space::Shutdown()
    {
        if (m_world != nullptr)
        {
            ShutdownWorld(m_space_manager->m_physics_system);
        }
        if (m_scene != nullptr)
        {
            ShutdownScene(m_space_manager->m_render_system);
        }
        if (m_object_manager != nullptr)
        {
            ShutdownManager(m_object_manager);
        }
        if (m_component_manager != nullptr)
        {
            ShutdownManager(m_component_manager);
        }
    }

    void Space::InitializeWorld(PhysicsSystem* physics_system)
    {
        m_world = physics_system->CreateWorld();
    }

    void Space::InitializeScene(RenderSystem* render_system)
    {
    }

    void Space::InitializeManager(ObjectFactory* obj_factory, ComponentFactory* cmp_factory)
    {
    }

    void Space::ShutdownWorld(PhysicsSystem* physics_system)
    {
        if (m_world != nullptr)
        {
            physics_system->RemoveWorld(m_world);
            m_world = nullptr;
        }
    }

    void Space::ShutdownScene(RenderSystem* render_system)
    {
        if (m_scene != nullptr)
        {
            render_system->RemoveScene(m_scene);
            m_scene = nullptr;
        }
    }

    void Space::ShutdownManager(ComponentManager* cmp_manager)
    {
        if (cmp_manager != nullptr)
        {
            cmp_manager->Shutdown();
            delete cmp_manager;
            cmp_manager = nullptr;
        }
    }

    void Space::ShutdownManager(ObjectManager* obj_manager)
    {
        if (obj_manager != nullptr)
        {
            obj_manager->Shutdown();
            delete obj_manager;
            obj_manager = nullptr;
        }
    }
}
