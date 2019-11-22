#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <vector>
#include "RigidBody.hpp"
#include "../Utility/PhysicsDef.hpp"
#include "../BroadPhase/BroadPhase.hpp"

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

        void SetBroadPhaseMode(BroadPhaseMode mode);

        ColliderPrimitive* CreateCollider(ColliderSet* collider_set, ColliderType type) const;

        RigidBody* AddRigidBody(RigidBody* body);
        ColliderSet* AddColliderSet(ColliderSet* set);
        void AddPrimitive(ColliderPrimitive* collider_primitive) const;

    private:
        

    private:
        BroadPhaseMode m_mode = BroadPhaseMode::DynamicBVH;
        BroadPhase* m_broad_phase = nullptr;

        std::vector<ColliderSet*> m_collider_sets;
        std::vector<RigidBody*>   m_rigid_bodies;
    };
}
