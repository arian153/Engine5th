#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <vector>
#include "RigidBody.hpp"
#include "../Utility/PhysicsDef.hpp"
#include "../BroadPhase/BroadPhase.hpp"
#include "../Resolution/ContactManifold.hpp"
#include <unordered_map>
#include "../../Graphics/DataType/Color.hpp"
#include "../Utility/FrictionUtility.hpp"

namespace Engine5
{
    class Force;
    class Constraint;
    class ManifoldTable;
    class ConstraintUtility;

    class World
    {
    public:
        World();
        ~World();

        void Initialize();
        void Update(Real dt);
        void Shutdown();
        void Render() const;

        void SetBroadPhaseMode(eBroadPhaseMode mode);
        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);
        void SetDrawFlagGJK(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagEPA(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagContact(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagPrimitive(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagBP(bool b_draw, const Color& color = ColorDef::Pure::White);

        ColliderPrimitive* CreateCollider(ColliderSet* collider_set, eColliderType type) const;

        RigidBody*   AddRigidBody(RigidBody* body);
        ColliderSet* AddColliderSet(ColliderSet* set);
        void         AddPrimitive(ColliderPrimitive* collider_primitive) const;
        void AddConstraint(Constraint* constraint) const;
        void AddForce(Force* force) const;

        void RemoveRigidBody(RigidBody* body);
        void RemoveColliderSet(ColliderSet* set);
        void RemovePrimitive(ColliderPrimitive* collider_primitive) const;
        void RemoveConstraint(Constraint* constraint) const;
        void RemoveForce(Force* force) const;

        void SetVelocityIteration(size_t iteration) const;
        void SetPositionIteration(size_t iteration) const;
        void SetWarmStarting(bool b_warm_starting) const;

        FrictionUtility*   GetFrictionUtility() const;
        ConstraintUtility* GetConstraintUtility() const;

    private:
        eBroadPhaseMode    m_mode               = eBroadPhaseMode::DynamicBVH;
        BroadPhase*        m_broad_phase        = nullptr;
        NarrowPhase*       m_narrow_phase       = nullptr;
        Resolution*        m_resolution_phase   = nullptr;
        ManifoldTable*     m_manifold_table     = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;

        std::vector<ColliderSet*> m_collider_sets;
        std::vector<RigidBody*>   m_rigid_bodies;
        std::list<ColliderPair>   m_pairs;

        ColorFlag m_draw_gjk;
        ColorFlag m_draw_epa;
        ColorFlag m_draw_contact;
        ColorFlag m_draw_primitive;
        ColorFlag m_draw_broad_phase;
    };
}
