#pragma once
#include <vector>
#include "../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class World;

    class PhysicsSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        World* CreateWorld();
        void RemoveWorld(World* world);


    private:
        std::vector<World*> m_worlds;

    };
}
