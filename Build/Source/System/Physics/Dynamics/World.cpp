#include "World.hpp"
#include "../BroadPhase/DynamicBVH.hpp"
#include "../BroadPhase/NSquared.hpp"
#include "../BroadPhase/GridPartition.hpp"
#include "../NarrowPhase/NarrowPhase.hpp"
#include "../NarrowPhase/ManifoldTable.hpp"
#include "ColliderSet.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../Resolution/Resolution.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Core/Utility/CoreUtility.hpp"

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
        if (m_broad_phase == nullptr)
        {
            switch (m_mode)
            {
            case eBroadPhaseMode::DynamicBVH:
                m_broad_phase = new DynamicBVH();
                break;
            case eBroadPhaseMode::StaticBVH:
                //m_broad_phase = new StaticBVH();
                break;
            case eBroadPhaseMode::NSquared:
                m_broad_phase = new NSquared();
                break;
            case eBroadPhaseMode::GridPartition:
                m_broad_phase = new GridPartition();
                break;
            default:
                m_broad_phase = new DynamicBVH();
                break;
            }
            m_broad_phase->Initialize();
        }
        m_manifold_table = new ManifoldTable();
        m_narrow_phase   = new NarrowPhase();
        m_narrow_phase->Initialize();
        m_resolution_phase = new Resolution();
        m_resolution_phase->Initialize();
    }

    void World::Render() const
    {
        if (m_draw_primitive.b_flag || m_draw_broad_phase.b_flag)
        {
            m_broad_phase->Render(m_primitive_renderer, m_draw_broad_phase, m_draw_primitive);
        }
        if (m_draw_gjk.b_flag || m_draw_epa.b_flag)
        {
            m_narrow_phase->Render(m_draw_gjk, m_draw_epa);
        }
        if (m_draw_contact.b_flag)
        {
            m_resolution_phase->Render(m_draw_contact);
        }
    }

    void World::Update(Real dt)
    {
        //broad phase
        m_broad_phase->Update(dt);
        m_broad_phase->ComputePairs(m_pairs);
        //narrow phase
        m_narrow_phase->GenerateContact(m_pairs, m_manifold_table);
        //resolution
        m_resolution_phase->SolveConstraints(m_manifold_table, &m_rigid_bodies, dt);
    }

    void World::Shutdown()
    {
        if (m_resolution_phase != nullptr)
        {
            m_resolution_phase->Shutdown();
            delete m_resolution_phase;
            m_resolution_phase = nullptr;
        }
        if (m_narrow_phase != nullptr)
        {
            m_narrow_phase->Shutdown();
            delete m_narrow_phase;
            m_narrow_phase = nullptr;
        }
        if (m_manifold_table != nullptr)
        {
            m_manifold_table->Shutdown();
            delete m_manifold_table;
            m_manifold_table = nullptr;
        }
        m_pairs.clear();
        for (auto& body : m_rigid_bodies)
        {
            body->Shutdown();
            delete body;
            body = nullptr;
        }
        m_rigid_bodies.clear();
        for (auto& collider : m_collider_sets)
        {
            collider->Shutdown();
            delete collider;
            collider = nullptr;
        }
        m_collider_sets.clear();
        if (m_broad_phase != nullptr)
        {
            m_broad_phase->Shutdown();
            delete m_broad_phase;
            m_broad_phase = nullptr;
        }
    }

    void World::SetBroadPhaseMode(eBroadPhaseMode mode)
    {
        if (mode != m_mode && m_broad_phase != nullptr)
        {
            BroadPhase* broad_phase;
            switch (mode)
            {
            case eBroadPhaseMode::DynamicBVH:
                broad_phase = new DynamicBVH();
                break;
            case eBroadPhaseMode::NSquared:
                broad_phase = new NSquared();
                break;
            case eBroadPhaseMode::GridPartition:
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

    void World::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        if (primitive_renderer != nullptr)
        {
            m_primitive_renderer = primitive_renderer;
            m_narrow_phase->SetPrimitiveRenderer(primitive_renderer);
            m_resolution_phase->SetPrimitiveRenderer(primitive_renderer);
        }
    }

    void World::SetDrawFlagGJK(bool b_draw, const Color& color)
    {
        m_draw_gjk.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_gjk.color  = color;
    }

    void World::SetDrawFlagEPA(bool b_draw, const Color& color)
    {
        m_draw_epa.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_epa.color  = color;
    }

    void World::SetDrawFlagContact(bool b_draw, const Color& color)
    {
        m_draw_contact.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_contact.color  = color;
    }

    void World::SetDrawFlagPrimitive(bool b_draw, const Color& color)
    {
        m_draw_primitive.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_primitive.color  = color;
    }

    void World::SetDrawFlagBP(bool b_draw, const Color& color)
    {
        m_draw_broad_phase.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_broad_phase.color  = color;
    }

    ColliderPrimitive* World::CreateCollider(ColliderSet* collider_set, eColliderType type) const
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

    void World::RemoveRigidBody(RigidBody* body)
    {
        auto found = std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), body);
        m_rigid_bodies.erase(found);
    }

    void World::RemoveColliderSet(ColliderSet* set)
    {
        auto found = std::find(m_collider_sets.begin(), m_collider_sets.end(), set);
        m_collider_sets.erase(found);
    }

    void World::RemovePrimitive(ColliderPrimitive* collider_primitive) const
    {
        m_broad_phase->Remove(collider_primitive->GetBoundingVolume());
    }

    void World::SetVelocityIteration(size_t iteration) const
    {
        m_resolution_phase->m_velocity_iteration = iteration;
    }

    void World::SetPositionIteration(size_t iteration) const
    {
        m_resolution_phase->m_position_iteration = iteration;
    }

    void World::SetWarmStarting(bool b_warm_starting) const
    {
        m_resolution_phase->m_b_warm_starting = b_warm_starting;
    }

    FrictionUtility* World::GetFrictionUtility() const
    {
        return &m_resolution_phase->m_friction_utility;
    }

    ConstraintUtility* World::GetConstraintUtility() const
    {
        return &m_resolution_phase->m_constraint_utility;
    }
}
