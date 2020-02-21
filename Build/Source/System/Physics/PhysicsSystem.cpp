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
        for (auto& world : m_worlds)
        {
            world->Update(dt);
        }
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
        world->Initialize();
        return world;
    }

    void PhysicsSystem::RemoveWorld(World* world)
    {
        if (world != nullptr)
        {
            m_worlds.erase(std::find(m_worlds.begin(), m_worlds.end(), world));
            world->Shutdown();
            delete world;
            world = nullptr;
        }
    }
}
