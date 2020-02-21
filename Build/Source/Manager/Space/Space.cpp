#include "Space.hpp"
#include "../../System/Physics/Dynamics/World.hpp"
#include "../../System/Physics/PhysicsSystem.hpp"
#include "../../System/Graphics/RenderSystem.hpp"

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
        

    }

    void Space::Update(Real dt)
    {
        //update world

        //update scene


    }

    void Space::Shutdown()
    {
    }

    void Space::InitializeWorld(PhysicsSystem* physics_system)
    {
        m_world = physics_system->CreateWorld();
    }

    void Space::InitializeScene(RenderSystem* render_system)
    {
        
    }

    void Space::InitializeManager(ObjectFactory* obj_factory, ComponentFactory* cmp_factory)
    {
    }
}
