#include "PhysicsSystem.hpp"
#include "Dynamics/World.hpp"

namespace Engine5
{
    PhysicsSystem::PhysicsSystem()
    {
    }

    PhysicsSystem::~PhysicsSystem()
    {
    }

    void PhysicsSystem::Initialize()
    {
    }

    void PhysicsSystem::Shutdown()
    {
        for (auto& world : m_worlds)
        {
            world->Shutdown();
            delete world;
            world = nullptr;
        }
        m_worlds.clear();
    }

    World* PhysicsSystem::CreateWorld()
    {
        World* world = new World();
        m_worlds.push_back(world);
        return world;
    }

    void PhysicsSystem::RemoveWorld(World* world)
    {
        if (world != nullptr)
        {
            auto found = std::find(m_worlds.begin(), m_worlds.end(), world);
            m_worlds.erase(found);
            world->Shutdown();
            delete world;
            world = nullptr;
        }
    }
}
