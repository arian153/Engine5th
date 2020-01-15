#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include <list>
#include <unordered_map>

namespace Engine5
{
    enum class CollisionState : int
    {
        None = 0
      , Start
      , Persist
      , End
      , Invalid
    };

    class ColliderPrimitive;

    class CollisionData
    {
    public:
        CollisionData();
        ~CollisionData();

    public:
        CollisionState     state      = CollisionState::None;
        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;
        Vector3            collision_point_a;
        Vector3            collision_point_b;
    };

    class CollisionDataTable
    {
    public:
        CollisionDataTable();
        ~CollisionDataTable();

        void Initialize();
        void Shutdown();

        void SendHasCollision(ColliderPrimitive* a, ColliderPrimitive* b, bool was_collision, const Vector3& collision_point_a, const Vector3& collision_point_b);
        void SendNotCollision(ColliderPrimitive* a, ColliderPrimitive* b, bool was_collision);
        void SendInvalidCollision(ColliderPrimitive* a, ColliderPrimitive* b);

        void   ValidateKeyMap(ColliderPrimitive* a, ColliderPrimitive* b);
        size_t GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b);

        auto FindRelatedColliderPairList(ColliderPrimitive* key);

        std::list<CollisionData> FindCollisionDataList(ColliderPrimitive* key);
        CollisionState           FindCollisionState(ColliderPrimitive* a, ColliderPrimitive* b);

    public:
        std::unordered_multimap<size_t, CollisionData>                  m_collision_data_map;
        std::unordered_multimap<ColliderPrimitive*, ColliderPrimitive*> m_collision_data_key_map;
    };
}
