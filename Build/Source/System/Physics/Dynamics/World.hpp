#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <vector>
#include "RigidBody.hpp"
#include "../Utility/PhysicsDef.hpp"
#include "../BroadPhase/BroadPhase.hpp"
#include "../Resolution/ContactManifold.hpp"
#include <unordered_map>
#include "../../Graphics/Utility/Color.hpp"

namespace Engine5
{
    class FilteringPhase;

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
        void         AddPrimitive(ColliderPrimitive* collider_primitive) const;

    private:


    private:
        BroadPhaseMode m_mode = BroadPhaseMode::DynamicBVH;
        BroadPhase* m_broad_phase = nullptr;
        NarrowPhase* m_narrow_phase = nullptr;
        Resolution* m_resolution_phase = nullptr;

        std::vector<ColliderSet*>            m_collider_sets;
        std::vector<RigidBody*>              m_rigid_bodies;
        std::list<ColliderPair>              m_pairs;
        std::unordered_multimap<size_t, ContactManifold> m_manifold_table;
        FilteringPhase* m_collision_data_table = nullptr;

        ColorFlag m_draw_gjk;
        ColorFlag m_draw_epa;
        ColorFlag m_draw_contact;
        ColorFlag m_draw_collider;
        ColorFlag m_draw_bounding_volume;

    };
}
