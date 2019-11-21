#pragma once
#include "..//..//Math/Math.hpp"

namespace Engine5
{
    class ColliderSet;
    class ColliderPrimitive;

    class BoundingAABB
    {
    public:
        explicit BoundingAABB(ColliderPrimitive* collider = nullptr);
        BoundingAABB(const BoundingAABB& a, const BoundingAABB& b);
        ~BoundingAABB();

        void         Set(const Vector3& min, const Vector3& max);
        bool         Intersect(BoundingAABB* aabb) const;
        bool         Contains(const Vector3& point) const;
        bool         TestRayIntersection(const Ray& ray, Real& t, Real max_distance = -1.0f) const;
        bool         Contains(BoundingAABB* aabb) const;
        Real         Volume() const;
        Real         Growth(const BoundingAABB& aabb) const;
        Vector3      Center() const;
        Vector3      Size() const;
        Vector3      HalfSize() const;
        BoundingAABB Union(const BoundingAABB& aabb) const;
        //getter
        ColliderPrimitive* GetCollider() const;
        void*              GetUserData() const;
    private:
        friend class DynamicBVHNode;
        friend class DynamicBVH;
        friend class GridData;
        friend class GridPartition;

    private:
        Vector3            m_min;
        Vector3            m_max;
        ColliderSet*       m_collider_set = nullptr;
        ColliderPrimitive* m_collider     = nullptr;
        void*              m_userdata     = nullptr;
    };
}
