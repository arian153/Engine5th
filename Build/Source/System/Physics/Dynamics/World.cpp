#include "World.hpp"
#include "../BroadPhase/DynamicBVH.hpp"
#include "../BroadPhase/StaticBVH.hpp"
#include "../BroadPhase/NSquared.hpp"
#include "../BroadPhase/GridPartition.hpp"


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

    void World::SetBroadPhaseMode(BroadPhaseMode mode)
    {
        if (mode != m_mode)
        {
            BroadPhase* broad_phase;
            switch (mode)
            {
            case Engine5::BroadPhaseMode::DynamicBVH:
                broad_phase = new DynamicBVH();
                break;
            case Engine5::BroadPhaseMode::StaticBVH:
                broad_phase = new StaticBVH();
                break;
            case Engine5::BroadPhaseMode::NSquared:
                broad_phase = new NSquared();
                break;
            case Engine5::BroadPhaseMode::GridPartition:
                broad_phase = new GridPartition();
                break;
            default:
                broad_phase = new DynamicBVH();
                break;
            }
            m_broad_phase->Copy(broad_phase);
            m_broad_phase->Clear();
            delete m_broad_phase;
            m_broad_phase = broad_phase;
            m_mode        = mode;
        }
    }

    ColliderPrimitive* World::CreateCollider(ColliderSet* collider_set, ColliderType type) const
    {
        if (collider_set != nullptr)
        {
            auto primitive = collider_set->AddCollider(type);
            AddPrimitive(primitive);
            primitive->Initialize();
            return primitive;
        }
        return nullptr;
    }

    RigidBody* World::AddRigidBody(RigidBody* body)
    {
        m_rigid_bodies.push_back(body);
        return body;
    }

    ColliderSet* World::AddColliderSet(ColliderSet* set)
    {
        m_collider_sets.push_back(set);
        return set;
    }

    void World::AddPrimitive(ColliderPrimitive* collider_primitive) const
    {
        BoundingAABB* bounding_volume = new BoundingAABB(collider_primitive);
        m_broad_phase->Add(bounding_volume);
    }
}
