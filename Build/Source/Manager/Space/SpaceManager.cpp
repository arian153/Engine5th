#include "SpaceManager.hpp"
#include "Space.hpp"

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
    }

    void SpaceManager::Update(Real dt)
    {
        for (auto& space : m_active_spaces)
        {
            space->Update(dt);
        }
    }

    void SpaceManager::Shutdown()
    {
    }

    void SpaceManager::Activate(Space* space)
    {
    }

    void SpaceManager::Deactivate(Space* space)
    {
    }
}
