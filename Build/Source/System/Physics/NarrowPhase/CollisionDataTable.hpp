#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include <list>
#include <unordered_map>

namespace Engine5
{
    enum class CollisionState : int
    {
        None = 0, Start, Persist, End, Invalid
    };

    class ColliderPrimitive;
    class ContactManifold;
    class ColliderSet;
    class RigidBody;

    class CollisionStateData
    {
    public:
        RigidBody* body_a = nullptr;
        RigidBody* body_b = nullptr;
        CollisionState state = CollisionState::None;
    };

    class CollisionDataTable
    {
    public:
        CollisionDataTable();
        ~CollisionDataTable();

        void Initialize(std::unordered_multimap<size_t, ContactManifold>* data_table);
        void Shutdown();

        void SendHasCollision(RigidBody* a, RigidBody* b, bool was_collision);
        void SendNotCollision(RigidBody* a, RigidBody* b, bool was_collision);
        void SendInvalidCollision(RigidBody* a, RigidBody* b);

        void   ValidateKey(RigidBody* a, RigidBody* b);
        size_t GenerateKey(RigidBody* a, RigidBody* b);
        size_t GenerateKey(ColliderSet* a, ColliderSet* b);
        size_t GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b);
        size_t GenerateKey(ContactManifold* manifold);
        auto FindAssociatedPairs(RigidBody* a);
        auto FindCollisionDatas(RigidBody* a, RigidBody* b, size_t at = 0);
        auto FindColisionState(RigidBody* a, RigidBody* b);

    public:
        std::unordered_multimap<size_t, ContactManifold>* m_manifold_table = nullptr;
        std::unordered_multimap<size_t, CollisionStateData> m_state_table;
        std::unordered_multimap<RigidBody*, RigidBody*> m_key_table;
    };
}
