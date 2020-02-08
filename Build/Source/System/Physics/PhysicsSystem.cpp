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

    void PhysicsSystem::Update(Real dt)
    {
    }

    void PhysicsSystem::Shutdown()
    {
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
            auto end = m_worlds.end();
            for (auto it = m_worlds.begin(); it != end;)
            {
                if (*it == world)
                {
                    m_worlds.erase(it++);
                    break;
                }
                ++it;
            }
            world->Shutdown();
            delete world;
            world = nullptr;
        }
    }
}
