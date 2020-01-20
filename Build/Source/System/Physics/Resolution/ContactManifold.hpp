#pragma once
#include "../../Math/Math.hpp"
#include "ContactPoint.hpp"

namespace Engine5
{
    class ContactPoint;

    class ContactManifold
    {
    public:
        ContactManifold();
        ~ContactManifold();
        void Set(const ContactManifold& manifold);
        void SetPersistentThreshold(Real threshold);
        void UpdateInvalidContact();
        void UpdateCurrentManifold(const ContactPoint& new_contact);
        void UpdateCollisionState();
        void CutDownManifold();
        size_t ContactsCount() const;

    private:
        Real DistanceFromPoint(const ContactPoint& contact, ContactPoint* p0);
        Real DistanceFromLineSegment(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1);
        Real DistanceFromTriangle(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2);
        bool OnTriangle(ContactPoint* point, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2);
        void CalculateNormal();

    private:
        friend class Resolution;
        friend class NarrowPhase;

    private:
        Real    persistent_threshold_squared = 4.0f;
        Vector3 cache_a_positional;
        Vector3 cache_a_rotational;
        Vector3 cache_b_positional;
        Vector3 cache_b_rotational;
        bool    is_collide = false;

        Vector3 normal;

        //data
        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;
        std::vector<ContactPoint> contacts;
    };
}
