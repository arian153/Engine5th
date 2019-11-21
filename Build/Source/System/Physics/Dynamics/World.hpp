#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <vector>
#include "RigidBody.hpp"
#include "../Utility/PhysicsDef.hpp"

namespace Engine5
{
    class World
    {
    public:
        World();
        ~World();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        RigidBody* AddRigidBody(RigidBody* body);

    private:
        std::vector<ColliderPrimitive*> m_coliders;
        std::vector<RigidBody*> m_bodies;
    };
}
