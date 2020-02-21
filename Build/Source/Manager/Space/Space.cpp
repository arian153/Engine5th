#include "Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Physics/PhysicsSystem.hpp"
#include "../../System/Graphics/RenderSystem.hpp"
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

    void Space::Initialize()
    {
        //maybe add a load data from file.
    }

    void Space::Update(Real dt) const
    {
        //Update Game Logic
        //Update World
        if (m_world != nullptr)
        {
            m_world->Update(dt);
        }
        //Update Animation
        //Update Scene
        if (m_scene != nullptr)
        {
            m_scene->Update(dt);
        }
    }

    void Space::Shutdown()
    {
        //maybe add a save data to file.
    }

    void Space::InitializeWorld(PhysicsSystem* physics_system)
    {
        if (m_world == nullptr)
        {
            m_world = physics_system->CreateWorld();
        }
    }

    void Space::InitializeScene(RenderSystem* render_system)
    {
        if (m_scene == nullptr)
        {
            m_scene = render_system->CreateScene();
        }
    }

    void Space::InitializeManager(ObjectFactory* obj_factory)
    {
        if (m_object_manager == nullptr)
        {
            m_object_manager = new ObjectManager();
            m_object_manager->Initialize(obj_factory);
        }
    }

    void Space::InitializeManager(ComponentRegistry* cmp_registry)
    {
        if (m_component_manager == nullptr)
        {
            m_component_manager = new ComponentManager();
            m_component_manager->Initialize(cmp_registry);
        }
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
