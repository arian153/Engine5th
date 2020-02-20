#include "SpaceManager.hpp"
#include "Space.hpp"
#include "SpaceFactory.hpp"

namespace Engine5
{
    SpaceManager::SpaceManager()
    {
    }

    SpaceManager::~SpaceManager()
    {
    }

    void SpaceManager::Initialize()
    {
        if (m_space_factory == nullptr)
        {
            m_space_factory = new SpaceFactory();
        }
        if (m_global_space == nullptr)
        {
            m_global_space = m_space_factory->Create();
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
        if (m_space_factory != nullptr)
        {
            delete m_space_factory;
            m_space_factory = nullptr;
        }
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
