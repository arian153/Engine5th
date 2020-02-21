#include "SpaceManager.hpp"
#include "Space.hpp"

namespace Engine5
{
    SpaceManager::SpaceManager(PhysicsSystem* physics, RenderSystem* renderer, ObjectFactory* obj, ComponentFactory* cmp)
        : m_physics_system(physics), m_render_system(renderer), m_object_factory(obj), m_component_factory(cmp)
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
            m_global_space->Initialize();
        }
    }

    void SpaceManager::Update(Real dt)
    {
        m_b_global_first ? UpdateFirst(dt) : UpdateLast(dt);
        if (m_b_next_order != m_b_global_first)
        {
            m_b_global_first = m_b_next_order;
        }
    }

    void SpaceManager::Shutdown()
    {
    }

    void SpaceManager::Activate(Space* space)
    {
        m_inactive_spaces.erase(std::find(m_inactive_spaces.begin(), m_inactive_spaces.end(), space));
        m_active_spaces.push_back(space);
    }

    void SpaceManager::Deactivate(Space* space)
    {
        m_active_spaces.erase(std::find(m_active_spaces.begin(), m_active_spaces.end(), space));
        m_inactive_spaces.push_back(space);
    }

    void SpaceManager::SetGlobalOrder(bool b_first)
    {
        m_b_next_order = b_first;
    }

    Space* SpaceManager::CreateSpace()
    {
        Space* space = new Space();
        m_active_spaces.push_back(space);
        space->Initialize();
        return space;
    }

    void SpaceManager::RemoveSpace(Space* space)
    {
        if (space != nullptr)
        {
            m_active_spaces.erase(std::find(m_active_spaces.begin(), m_active_spaces.end(), space));
            m_inactive_spaces.erase(std::find(m_inactive_spaces.begin(), m_inactive_spaces.end(), space));
            space->Shutdown();
            delete space;
            space = nullptr;
        }
    }

    void SpaceManager::UpdateFirst(Real dt)
    {
        m_global_space->Update(dt);
        for (auto& space : m_active_spaces)
        {
            space->Update(dt);
        }
    }

    void SpaceManager::UpdateLast(Real dt)
    {
        for (auto& space : m_active_spaces)
        {
            space->Update(dt);
        }
        m_global_space->Update(dt);
    }
}
