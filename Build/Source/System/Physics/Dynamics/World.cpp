#include "World.hpp"


namespace Engine5
{
    World::World()
    {
    }

    World::~World()
    {
    }

    void World::Initialize()
    {
    }

    void World::Update(Real dt)
    {
    }

    void World::Shutdown()
    {
    }

    RigidBody* World::AddRigidBody(RigidBody* body)
    {
        m_bodies.push_back(body);
        return body;
    }

   }
