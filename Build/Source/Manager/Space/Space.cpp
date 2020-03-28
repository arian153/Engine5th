#include "Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Physics/PhysicsSystem.hpp"
#include "../../System/Graphics/RenderSystem.hpp"
#include "../Object/ObjectManager.hpp"
#include "../Component/ComponentManager.hpp"
#include "../Resource/ResourceType/JsonResource.hpp"

namespace Engine5
{
    Space::Space()
    {
    }

    Space::~Space()
    {
    }

    void Space::Initialize(eSubsystemFlag flag, PhysicsSystem* physics_system, RenderSystem* render_system, ObjectFactory* obj_factory, ComponentRegistry* cmp_registry)
    {
        m_creation_flag = flag;
        //create component manager
        if (m_component_manager == nullptr && HasFlag(flag, eSubsystemFlag::ComponentManager))
        {
            m_component_manager = new ComponentManager();
            m_component_manager->Initialize(cmp_registry, this);
        }
        //create object manager
        if (m_object_manager == nullptr && HasFlag(flag, eSubsystemFlag::ObjectManager))
        {
            m_object_manager = new ObjectManager();
            m_object_manager->Initialize(obj_factory, this);
        }
        //create scene
        if (m_scene == nullptr && HasFlag(flag, eSubsystemFlag::Scene))
        {
            m_scene = render_system->CreateScene();
            m_scene->Initialize();
        }
        //create world
        if (m_world == nullptr && HasFlag(flag, eSubsystemFlag::World))
        {
            m_world = physics_system->CreateWorld();
            m_world->Initialize();
        }
    }

    void Space::Initialize(JsonResource* space_resource, PhysicsSystem* physics_system, RenderSystem* render_system, ObjectFactory* obj_factory, ComponentRegistry* cmp_registry)
    {
        m_space_resource = space_resource;
        space_resource->LoadSpaceFlag(this);
        //create component manager
        if (m_component_manager == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::ComponentManager))
        {
            m_component_manager = new ComponentManager();
            m_component_manager->Initialize(cmp_registry, this);
        }
        //create object manager
        if (m_object_manager == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::ObjectManager))
        {
            m_object_manager = new ObjectManager();
            m_object_manager->Initialize(obj_factory, this);
        }
        //create scene
        if (m_scene == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::Scene))
        {
            m_scene = render_system->CreateScene();
            m_scene->Initialize();
        }
        //create world
        if (m_world == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::World))
        {
            m_world = physics_system->CreateWorld();
            m_world->Initialize();
            if (m_scene != nullptr)
            {
                m_world->SetPrimitiveRenderer(m_scene->GetPrimitiveRenderer());
            }
        }

        space_resource->LoadData(this);
    }

    void Space::Shutdown(PhysicsSystem* physics_system, RenderSystem* render_system)
    {
        //maybe add a save data to file.
        //shutdown world
        if (m_world != nullptr && HasFlag(m_creation_flag, eSubsystemFlag::World))
        {
            physics_system->RemoveWorld(m_world);
        }
        //shutdown scene
        if (m_scene != nullptr && HasFlag(m_creation_flag, eSubsystemFlag::Scene))
        {
            render_system->RemoveScene(m_scene);
        }
        //shutdown object manager
        if (m_object_manager != nullptr && HasFlag(m_creation_flag, eSubsystemFlag::ObjectManager))
        {
            m_object_manager->Shutdown();
            delete m_object_manager;
            m_object_manager = nullptr;
        }
        //shutdown component manager
        if (m_component_manager != nullptr && HasFlag(m_creation_flag, eSubsystemFlag::ComponentManager))
        {
            m_component_manager->Shutdown();
            delete m_component_manager;
            m_component_manager = nullptr;
        }
    }

    void Space::ConnectSubsystem(ComponentManager* component_manager)
    {
        if (m_component_manager == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::ComponentManager) == false)
        {
            m_component_manager = component_manager;
        }
    }

    void Space::ConnectSubsystem(ObjectManager* object_manager)
    {
        if (m_object_manager == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::ObjectManager) == false)
        {
            m_object_manager = object_manager;
        }
    }

    void Space::ConnectSubsystem(Scene* scene)
    {
        if (m_scene == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::Scene) == false)
        {
            m_scene = scene;
        }
    }

    void Space::ConnectSubsystem(World* world)
    {
        if (m_world == nullptr && HasFlag(m_creation_flag, eSubsystemFlag::World) == false)
        {
            m_world = world;
        }
    }

    ObjectManager* Space::GetObjectManager() const
    {
        return m_object_manager;
    }

    ComponentManager* Space::GetComponentManager() const
    {
        return m_component_manager;
    }

    Scene* Space::GetScene() const
    {
        return m_scene;
    }

    World* Space::GetWorld() const
    {
        return m_world;
    }

    bool Space::IsSubsystemUpdate(eSubsystemFlag flag) const
    {
        return HasFlag(m_update_flag, flag);
    }

    bool Space::IsSubsystemFixedUpdate(eSubsystemFlag flag) const
    {
        return HasFlag(m_fixed_update_flag, flag);
    }
}
