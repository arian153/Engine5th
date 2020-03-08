#include "SpaceManager.hpp"
#include "Space.hpp"
#include "../../System/Physics/PhysicsSystem.hpp"

namespace Engine5
{
    SpaceManager::SpaceManager(PhysicsSystem* physics, RenderSystem* renderer, ObjectFactory* obj, ComponentRegistry* cmp)
        : m_physics_system(physics), m_render_system(renderer), m_object_factory(obj), m_component_registry(cmp)
    {
    }

    SpaceManager::~SpaceManager()
    {
    }

    void SpaceManager::Initialize()
    {
        if (m_global_space == nullptr)
        {
            m_global_space = new Space();
            m_global_flag  = eSubsystemFlag::ComponentManager | eSubsystemFlag::ObjectManager | eSubsystemFlag::Scene | eSubsystemFlag::World;
            m_global_space->Initialize(m_global_flag, m_physics_system, m_render_system, m_object_factory, m_component_registry);
        }
    }

    void SpaceManager::Shutdown()
    {
        for (auto& space : m_spaces)
        {
            space->Shutdown(m_physics_system, m_render_system);
            delete space;
            space = nullptr;
        }
        m_spaces.clear();
        if (m_global_space != nullptr)
        {
            m_global_space->Shutdown(m_physics_system, m_render_system);
            delete m_global_space;
            m_global_space = nullptr;
        }
    }

    Space* SpaceManager::GetGlobalSpace() const
    {
        return m_global_space;
    }

    Space* SpaceManager::CreateSpace(eSubsystemFlag flag)
    {
        Space* space = new Space();
        m_spaces.push_back(space);
        space->m_creation_flag = flag;
        space->Initialize(flag, m_physics_system, m_render_system, m_object_factory, m_component_registry);
        return space;
    }

    void SpaceManager::RemoveSpace(Space* space)
    {
        if (space == m_global_space)
        {
            return;
        }
        if (space != nullptr)
        {
            m_spaces.erase(std::find(m_spaces.begin(), m_spaces.end(), space));
            space->Shutdown(m_physics_system, m_render_system);
            delete space;
            space = nullptr;
        }
    }
}
