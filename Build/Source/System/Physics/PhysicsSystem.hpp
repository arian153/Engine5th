#pragma once
#include <vector>

namespace Engine5
{
    class World;

    class PhysicsSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem();

        void Initialize();
        void Shutdown();

        World* CreateWorld();
        void   RemoveWorld(World* world);

    private:
        std::vector<World*> m_worlds;
    };
}
