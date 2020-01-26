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

    class CollisionStateData
    {
    public:
        ColliderSet* set_a = nullptr;
        ColliderSet* set_b = nullptr;
        CollisionState state = CollisionState::None;
    };

    class FilteringPhase
    {
    public:
        FilteringPhase();
        ~FilteringPhase();

        void Initialize(std::unordered_multimap<size_t, ContactManifold>* data_table);
        void Shutdown();

        void SendHasCollision(ColliderSet* a, ColliderSet* b, bool was_collision);
        void SendNotCollision(ColliderSet* a, ColliderSet* b, bool was_collision);
        void SendInvalidCollision(ColliderSet* a, ColliderSet* b);

        size_t GenerateKey(ColliderSet* a, ColliderSet* b);
        size_t GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b);
        size_t GenerateKey(ContactManifold* manifold);
        auto FindAssociatedPairs(ColliderSet* a);
        auto FindCollisionDatas(ColliderSet* a, ColliderSet* b, size_t at = 0);
        auto FindColisionState(ColliderSet* a, ColliderSet* b);

        ContactManifold* FindManifold(ColliderSet* a, ColliderSet* b);
        ContactManifold* CreateManifold(ColliderSet* a, ColliderSet* b);
        bool HasManifold(ColliderSet* a, ColliderSet* b);
        void FilteringManifolds();
        size_t RegisterKey(ColliderSet* a, ColliderSet* b);
        void DeregisterKey(ColliderSet* a, ColliderSet* b);

    public:
        std::unordered_multimap<size_t, ContactManifold>* m_manifold_table = nullptr;
        std::unordered_multimap<size_t, CollisionStateData> m_state_table;
        std::unordered_multimap<ColliderSet*, ColliderSet*> m_key_table;
    };
}
